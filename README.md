# gParallel <img align="right" width="30%" src="./image/logo.png">

[![Build Status](https://www.travis-ci.org/galois-advertising/gparallel.svg?branch=master)](https://www.travis-ci.org/galois-advertising/gparallel)


`gparallel`��һ����Ծ��и������̺��߼��ĵ���ʽ��Ϣ����ϵͳ����ƵĲ���������ȿ�ܡ�����`Meta Programming`������������������Զ��Ƶ�������ϵ������`DAG(Directed acyclic graph)`�����в���������ȡ�

# Quick start

* ��������
    - g++8
    - boost_log-mt v1.70
    - gtest v1.10.0

## ���ر���test��demo

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

## ��gparallel��Ϊ����Ŀ��һ����

���Ƚ�`gparallel`�Լ�������`common`��`gtest`����Ϊ`git submodule`

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
Ȼ���޸�`CMakeLists.txt`�����룺

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

# ��������

���ڵ�����ҵ��ϵͳ����ϵͳ�������ڣ�ϵͳҵ�񻹱Ƚϼ򵥣�ÿ��`��������`����ʱ��Ҫִ�е�ҵ���߼�Ҳ�Ƚϵ�һ����ʱ���󼶱��`���ݱ���`�Ƚ��٣���Щ�����ĸ�ֵ˳����������ϵҲһĿ��Ȼ���������ݼ�����һ����ȫ���Ե�һ�����̣�û���κ�`�첽����`��

���ǣ����ſ�������Խ��Խ�࣬��Ҷ�����������Լ���`ҵ���߼�`���µ�`���ݱ���`����ʱ�������ӵ��˼��ٸ�������֮��ĸ�ֵ˳����������ϵ��ʼ��ø��ӣ�һЩ�����߼��Ѿ�����ô����⡣���ʱ�򼴱�����ע�ͣ�Ҳû��һ������˵�����Щ����֮���������ϵ�Լ���Щҵ���߼�֮���ִ��˳�򣬼�ֱ����һ�����顣

ÿ���µĿ��������ı�����ÿ���Ų����ⶼ��ʱ����������Ǳ������ڹ�˾��ϵͳ��״��500�˲��뿪���������д��룬�켶��ĵ������¡���Ϊ��ӵ����߹��ϵͳ֮һ�������֯���ӵ�ҵ���߼�����δ�Ÿ����͵����ݣ������Ϊʲô��Ҫ`gparallel`.

`gparallel`��һ�����`DAG(Directed acyclic graph)`��������ȿ�ܡ�`DAG`�ڼ�����������Ź㷺��Ӧ�ã������ڴ����ݼ����п���ʹ��DAGָ��Hadoop�����ִ��˳��ȵȡ�����������Ҳ���㷺Ӧ�ã���Դ������DAG-based���ȿ��Ҳ��ʤö�١��������м������п�ܶ����������õķ�ʽ���ɵ���DAGͼ������Ƚϳ�����[cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow)��

<img align="right" width="20%" src="./image/cpp-task.png">
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

�����������Ҫ����Ԥ�ڵ�DAGͼ��Ҫ�˹���ʽ����ÿ�����ڵ�֮���������ϵ�����ַ�ʽ���е������Ƚ�ֱ�ۣ�����ȱ��Ҳ�����ԣ�
* ���д��������ʱ���˹�����DAGͼ�Ƚ����Ѳ������׳���
* ��ҵ�����кܶ�ҵ�񣬲�����������������������������������Ҫ���Ѵ���ʱ������������֮����������Ӷ�ת��Ϊ��������

# gparallel��ν������

gparallel����Ҫ˼����3����

* `���ݻ���`�����������ݳ�Ա������ҵ���߼�������״̬������Ϊ��ͬ�ļ��ϡ�
* `�����Ƶ�`�������еĴ����߼������չ��ܻ���Ϊ��ͬ��task�������Զ��Ƶ�task֮���������ϵ������DAG����ͼ��
* `�첽����`��ͨ��thread����coroutine����task���е��ȡ�

## ���ݻ���

�ڼ���ϵͳ�У�`����`һ�����ָ���������������洢�м��������ս���ı�����gparallel��Ҫ��2���Ƕ�ȥ���л��֣�

* `����ҵ���߼�`���Ѳ�ͬҵ���߼�����Ҫ�õ������ݻ���Ϊ��ͬ�ļ��ϡ������в�ͬ�Ĺ��Ӫ����Ʒ�����Զ����Լ���User��Plan��Ad�ļ��ϣ��Լ�һЩ�洢���м����ı�����Ϊ�˷��������������ô�д��ĸ����ʾ����ҵ���߼����ֳ��������ݼ��ɣ�����`A`��`B`��`C`�ȵȡ�

* `��������״̬`������һ�������ڷ�����ͬһ��ҵ������ݼ���(A)���ڲ�ͬ�Ľ׶Σ��ֿ��Ի���Ϊ��ͬ��״̬������һ�������У���ʼ״̬�ǿ�(empty)����������Ժ���N�����(inited)���־�����һ�ΰ���CTR������(ranked)���������һ�νض�(cut)����ô��Ӧ�ĸ�״̬`A_empty`��`A_inited`��`A_ranked`��`A_cut`��Ϊ�˷�����⣬���ǰѶ�������״̬�����ݽṹ������`meta`��ǰ���ᵽ��`A_empty`��`A_inited`��`A_ranked`��`A_cut`����`meta`����gparallel�У�������[`DECL_META`](include/meta.h)��������һ��`meta`��

���gparallel�����ݵ�2��ݽ����ַ�ʽ�ǳ���Ҫ����Ϊgparallel��DAG�Զ��Ƶ��������������ڲ�ͬ��meta��

## �����Ƶ�

## �첽����