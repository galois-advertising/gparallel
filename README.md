# gParallel <img align="right" width="30%" src="./image/logo.png">

[![Build Status](https://www.travis-ci.org/galois-advertising/gparallel.svg?branch=master)](https://www.travis-ci.org/galois-advertising/gparallel)


`gparallel`是一个针对具有复杂流程和逻辑的单体式信息检索系统而设计的并行任务调度框架。基于`Meta Programming`根据任务的输入和输出自动推导依赖关系，生成`DAG(Directed acyclic graph)`并进行并行任务调度。

# Quick start

* 编译依赖
    - g++8
    - boost_log-mt v1.70
    - gtest v1.10.0

## 下载编译test和demo

```shell
$ git clone git@github.com:galois-advertising/gparallel.git
$ cd gparallel
$ git submodule update --init --recursive
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./test
$ ./demo
```

## 将gparallel作为你项目的一部分

首先将`gparallel`以及依赖的`common`和`gtest`设置为`git submodule`

```shell
cd your-project
git add submodule -b master https://github.com/galois-advertising/gparallel
git add submodule -b master https://github.com/galois-advertising/common
git add submodule https://github.com/google/googletest.git gtest
cd gtest
git checkout release-1.10.0
git add gtest
git commit -m "Add gparallel"
```
然后修改`CMakeLists.txt`，加入：

```cmake
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/common/util/include")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/gparallel/include")
IF (NOT TARGET common)
    ADD_SUBDIRECTORY(common)
ENDIF()

IF (NOT TARGET gparallel)
    ADD_SUBDIRECTORY(gparallel)
ENDIF()
ADD_EXECUTABLE(your-bin ...)
TARGET_LINK_LIBRARIES(your-bin common gparallel)
```

# 背景介绍

对于单体型业务系统，在系统建立初期，系统业务还比较简单，每次`检索请求`到来时需要执行的业务逻辑也比较单一，此时请求级别的`数据变量`比较少，这些变量的赋值顺序与依赖关系也一目了然，整个数据检索是一个完全线性的一个过程，没有任何`异步操作`。

但是，随着开发的人越来越多，大家都在上面加入自己的`业务逻辑`和新的`数据变量`，此时变量增加到了几百个，变量之间的赋值顺序与依赖关系开始变得复杂，一些代码逻辑已经不那么好理解。这个时候即便是有注释，也没有一个人能说清楚这些变量之间的依赖关系以及这些业务逻辑之间的执行顺序，简直就是一团乱麻。

每次新的开发都如履薄冰，每次排查问题都耗时耗力。这就是笔者所在公司的系统现状，500人参与开发，百万行代码，天级别的迭代更新。作为最复杂的在线广告系统之一，如何组织繁杂的业务逻辑，如何存放各类型的数据，这就是为什么需要`gparallel`.

`gparallel`是一款基于`DAG(Directed acyclic graph)`的任务调度框架。`DAG`在计算机领域有着广泛的应用，例如在大数据计算中可以使用DAG指导Hadoop任务的执行顺序等等。在软件设计中也被广泛应用，开源社区中DAG-based调度框架也不胜枚举。但是其中几乎所有框架都采用了配置的方式生成调度DAG图，例如比较常见的有[cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow)。例如在一个拥有四个任务的调度系统中，`cpp-taskflow`需要通过下面方式来配置DAG。

```cpp
    auto [A, B, C, D] = taskflow.emplace(
      [] () { std::cout << "TaskA\n"; },
      [] () { std::cout << "TaskB\n"; },
      [] () { std::cout << "TaskC\n"; },
      [] () { std::cout << "TaskD\n"; }
    );
    A.precede(B);  // A runs before B 
    A.precede(C);  // A runs before C 
    B.precede(D);  // B runs before D 
    C.precede(D);  // C runs before D 
}
```
<img align="right" width="20%" src="./image/cpp-task.png">

上面代码中想要生成预期的DAG图需要人工显式定义每两个节点之间的依赖关系，这种方式虽然理解比较直观，但是缺点也非常明显：

* **在有大量任务的时候，人工定义DAG图比较困难并且容易出错。现实中的业务系统一般是多人同时开发，这就需要模块owner对所有的任务节点之间的依赖关系进行人工梳理，可维护性较差。**
* **工业环境中很多业务，往往以数据流驱动的方式表达会更加清晰，这就需要花费大量时间来将系统逻辑从数据驱动表示强行转化为任务驱动表示来适配调度系统，耗时耗力。**

# gparallel如何解决问题

gparallel的主要思想有3个：

* `数据划分`：将所有数据成员，按照业务逻辑和数据状态，划分为不同的集合。
* `依赖推导`：将所有的代码逻辑，按照功能划分为不同的task，并且自动推导task之间的依赖关系，建立DAG调度图。
* `任务调用`：通过拓扑排序，将`DAG`转化为`偏序`表示，并使用thread或者coroutine对task进行调度。

## 数据划分

在检索系统中，`数据`一般就是指检索过程中用来存储中间结果和最终结果的变量。gparallel主要从2个角度去进行划分：

* `按照业务逻辑`：把不同业务逻辑所需要用到的数据划分为不同的集合。比如有不同的广告营销产品，各自都有自己的User、Plan和Ad的集合，以及一些存储数中间结果的变量。为了方便描述，我们用大写字母来表示按照业务逻辑划分出来的数据即可，例如`A`、`B`、`C`等等。

* `按照数据状态`：更进一步，对于服务于同一个业务的数据集合(A)，在不同的阶段，又可以划分为不同的状态。例如一个广告队列，初始状态是空(empty)，经过填充以后有N条广告(inited)，又经过了一次按照CTR的排序(ranked)，最后经历了一次截断(cut)，那么对应四个状态`A_empty`，`A_inited`，`A_ranked`，`A_cut`。为了方便理解，我们把定义数据状态的数据结构，叫做`meta`。前面提到的`A_empty`，`A_inited`，`A_ranked`，`A_cut`都是`meta`。在gparallel中，我们用[`DECL_META`](include/meta.h)宏来定义一个`meta`。

理解gparallel对数据的2层递进划分方式非常重要，因为gparallel的DAG自动推导过程正是依赖于不同的meta。

现实中的系统中，数据成员一般放置在一个叫做`context`或者`thread_data`的结构体中。顾名思义，这些数据的作用范围就是一次请求，一个比较常见的设计是一次请求由线程池中的一个线程来独立负责，所以请求级别的数据，往往也是线程级别的数据。这个`context`或者`thread_data`的类型，我们定义为`meta_storage_t`，即所有meta用到的数据，都统一存储在这里。

`meta`对`meta_storage_t`子集的一个指定，通过定义`getter`和`setter`来实现的，如果定义了`getter`和`setter`就代表这个`meta`中包含这个数据成员。子集之间也可以互相包含，原理与面向对象中的`继承`是一样的。同理，如果一个任务依赖于一个`meta`，则也同样依赖于这个`meta`的父`meta`。`继承`机制的主要目的是为了避免重复定义集合的元素，增加代码的可维护性。通过下面的例子可以理解`meta_storage_t`、`meta`和`继承`的关系。

<div><img align="center" width="75%" src="./image/meta.png"></div>

## 任务定义

在`gparallel`中，使用一个函数表示一个具体的任务，函数的参数表示任务的`输入`和`输出`。任何一个`meta`既可以作为`输入`，也可以作为`输出`。这里引入2个模版包装器`input`和`output`。如果`meta`用`input`包装，则任务函数会将其当作一个输入数据，同理如果用`output`包装，则会当作输出。

表示任务的函数，必须定义为类的静态成员函数，函数名字必须为`process`，返回类型为`void`。例如：

```cpp
struct DemoNode {
    void process(input<meta_a> a, input<meta_b> b, output<meta_c> c) {
        // process code
        c->mutable_business_c() = a->get_business_a() + b->get_business_b();
    }
}
```
上面的`DemoNode`实现了将`business_a`和`business_b`的和赋值给`business_c`的逻辑。
`process`函数可以拥有任意多个`输入`和任意多个`输出`。其中没有`输入`的节点作为起始节点之一，没有`输出`的节点作为终止节点之一。

## 任务调度

通过`register_node`函数将所有节点注册到`DAG`中后，使用`setup_dag_schema`函数可自动实现节点依赖分析以及`DAG`生成。针对生成的`DAG`进行`拓扑排序`，就能得到任务调度顺序。后面可根据具体情况，实现多线程或者多协程调度。

```cpp
dag_schema<thread_data> nodes;
register_node<thread_data, node_a, node_b, node_c ...>::reg(nodes);
setup_dag_schema<thread_data>(nodes);
if (auto tasks = topological_sort<thread_data>(nodes); tasks) {
    for (auto task : tasks.value()) {
        task->mutable_executor()(&td);
    }
}
```

<center><img align="center" src="./image/dispatch.png"></center>

# gparallel实战

在本小结中，会从一个现实的场景来描述`gparallel`的使用逻辑。

> 问题描述：对一个指定的广告队列，分别请求其`CTR`(点击率)值和`CPM`(千次展示成本)并填充到广告的对应字段，最后分别按照`CTR`和`CPM`进行排序后生成2个新的广告队列供下游使用。

上述流程是广告检索系统里面一个比较典型的逻辑，完整的代码在[./demo/advprocess.cpp](./demo/advprocess.cpp)。

我们首先梳理一下所需要用到的数据对象：
数据名称 | 类型 |  含义  
-|-|-
advs_original|  advlist_t |  原始的广告队列 |
ctr_data |  advlist_t | 模型返回的ctr数据|
cpm_data |  advlist_t |  模型返回的cpm数据|
advs_ctr_ordered | ctr_response_t |  输出的ctr排序的广告队列|
advs_cpm_ordered | cpm_response_t |  输出的cpm排序的广告队列|

根据上面的定义，我们定义`meta_storage_t`：

```cpp
class thread_data {
public:
    advlist_t advs_original;
    advlist_t advs_ctr_ordered;
    advlist_t advs_cpm_ordered;
    ctr_response_t ctr_data;
    cpm_response_t cpm_data;
};
```

根据问题的描述，我们可以很容易总结出5个子流程，每个子流程都对应一个数据处理节点：

同理，每个子流程的输入于输出可以定义为：
<center>
<table>
    <thead>
        <tr>
            <th>流程</th><th>节点名称</th><th>输入</th><th>输出</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>获取CTR值</td><td>get_ctr_node</td><td>advs_original</td><td>ctr_data</td>
        </tr>
        <tr>
            <td>获取CPM值</td><td>get_cpm_node</td><td>advs_original</td><td>cpm_data</td>
        </tr>
        <tr>
            <td>填充字段</td><td>fill_node</td>
            <td>advs_original &amp; ctr_data &amp; cpm_data</td>
            <td>advs_original(filled)</td>
        </tr>
        <tr>
            <td>生成CTR排序队列</td><td>gen_ctr_node</td><td>advs_original(filled)</td>
            <td>advs_ctr_ordered</td>
        </tr>
        <tr>
            <td>生成CPM排序队列</td><td>gen_cpm_node</td>
            <td>advs_original(filled)</td><td>advs_cpm_ordered</td>
        </tr>
    </tbody>
</table>
<center>

<img align="right" width="50%" src="./image/metas.png">


