# gParallel <img align="right" width="30%" src="./image/logo.png">

[![Build Status](https://www.travis-ci.org/galois-advertising/gparallel.svg?branch=master)](https://www.travis-ci.org/galois-advertising/gparallel)


gparallel是一个针对逻辑比较复杂的信息检索系统而设计的并行任务调度框架。基于DAG(Directed acyclic graph)进行并行任务调度，且自动根据任务的输入和输出推导依赖关系。

# Quick start

* 编译依赖
    - g++8
    - boost-v1.70
    - gtest-v1.10.0

下载并编译test

```shell
$ git clone git@github.com:galois-advertising/gparallel.git
$ cd gparallel
$ git submodule update --init --recursive
$ mkdir build
$ cd build
$ cmake ..
$ make gparallel_test
$ ./gparallel_test
```

将gparallel作为你项目的一部分

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
并且修改`CMakeLists.txt`，加入：
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

每次新的开发都如履薄冰，每次排查问题都耗时耗力。这就是笔者所在公司的系统现状，500人参与开发，百万行代码，天级别的迭代更新。作为最复杂的在线广告系统之一，如何组织繁杂的业务逻辑，如何存放各类型的数据，这就是为什么需要gparallel.

# gparallel如何解决问题

gparallel的主要思想有3个：

* `数据划分`：将所有数据成员，按照业务逻辑和数据状态，划分为不同的集合。
* `任务推导`：将所有的代码逻辑，按照功能划分为不同的task，并且自动推导task之间的依赖关系，建立DAG调度图。
* `异步调用`：通过thread或者coroutine，对task进行调度。

## 数据划分

在检索系统中，`数据`一般就是指检索过程中用来存储中间结果和最终结果变量。gparallel主要从2个角度去进行划分：

* `按照业务逻辑`：把不同业务逻辑所需要用到的数据划分为不同的集合。比如有不同的广告营销产品，各自都有自己的User、Plan和Ad的集合，以及一些存储数中间结果的变量。为了方便描述，我们用大写字母来表示按照业务逻辑划分出来的数据即可，例如`A`、`B`、`C`等等。

* `按照数据状态`：更进一步，对于服务于同一个业务的数据集合(A)，在不同的阶段，又可以划分为不同的状态。例如一个广告队列，初始状态是空(empty)，经过填充以后有N条广告(inited)，又经过了一次按照CTR的排序(ranked)，最后经历了一次截断(cut)，那么对应四个状态`A_empty`，`A_inited`，`A_ranked`，`A_cut`。为了方便理解，我们把定义数据状态的数据结构，叫做`meta`。前面提到的`A_empty`，`A_inited`，`A_ranked`，`A_cut`都是`meta`。在gparallel中，我们用[`DECL_META`](include/meta.h)宏来定义一个`meta`。

理解gparallel对数据的2层递进划分方式非常重要，因为gparallel的DAG自动推导过程正是依赖于不同的meta。

## 任务推导

## 异步调用