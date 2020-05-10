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

但是，随着开发的人越来越多，大家都在上面加入自己的`业务逻辑`和新的`数据变量`，此时变量增加到了几十个，变量之间的赋值顺序与依赖关系开始变得复杂，一些代码逻辑已经不那么好理解，于此同时因为增加了`外部功能`调用，所以运行出现了大量`同步等待`，系统`平响`开始上升。面对这种情况，代码的管理者强制要求CI人员一定要添加足够的注释以便后面的开发人员能够明白代码和数据之间运行的逻辑。

几个月后，大家突然发现请求级别的`数据变量`已经多达几百个！而且`业务逻辑`也变得更加复杂。这个时候即便是有注释，也没有一个人能说清楚这些变量之间的依赖关系以及这些业务逻辑之间的执行顺序，简直就是一团乱麻。

每次新的开发都如履薄冰，每次排查问题都耗时耗力。

对，这就是笔者所在公司的系统现状，500人参与开发，百万行代码，天级别的迭代更新。作为最复杂的在线广告系统之一，如何组织繁杂的业务逻辑，如何存放各类型的数据，这就是为什么需要gparallel.

# gparallel如何解决问题

gparallel的主要思想有3个：

* `数据划分`：将所有数据成员，按照业务逻辑和数据状态，划分为不同的集合。
* `任务推导`：将所有的代码逻辑，按照功能划分为不同的task，并且自动推导task之间的依赖关系，建立DAG调度图。
* `异步调用`：通过thread或者coroutine，对task进行调度。

## 数据划分

在检索系统中，`数据`一般就是指检索过程中用来存储中间结果和最终结果变量。gparallel主要从2个角度去进行划分：

* `业务逻辑`：把不同业务逻辑所需要用到的数据划分为不同的集合。
* `数据状态`：根据数据的状态（或者说阶段）把同一份数据虚拟为不同的数据集合。

根据业务逻辑划分数据是比较常规的思路，每个数据集会是会
数据状态划分是指在同一个变量，在整个检过程中依此由未赋值，被赋值为A、被赋值为B的过程中，实际会产生3个集合。

## 任务推导

## 异步调用