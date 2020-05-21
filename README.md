# gParallel <img align="right" width="30%" src="./image/logo.png">

[![Build Status](https://www.travis-ci.org/galois-advertising/gparallel.svg?branch=master)](https://www.travis-ci.org/galois-advertising/gparallel)


`gparallel`��һ����Ծ��и������̻��߼��ĵ���ʽ��Ϣ����ϵͳ����ƵĲ���������ȿ�ܡ�ʹ��`Meta Programming`����������������������Զ��Ƶ�������ϵ������`DAG(Directed acyclic graph)`�����в���������ȡ�

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

���Ƚ�`gparallel`�Լ���������`common`��`gtest`����Ϊ`git submodule`��

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

�����޸�`CMakeLists.txt`�����룺

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

���ڵ�����ҵ��ϵͳ����ϵͳ�������ڣ�ϵͳҵ�񻹱Ƚϼ򵥣�ÿ��`��������`����ʱ��Ҫִ�е�ҵ���߼�Ҳ�Ƚϵ�һ����ʱ���󼶱��`���ݱ���`�Ƚ��٣���Щ�����ĸ�ֵ˳����������ϵҲһĿ��Ȼ��ϵͳ������ά����

���ǣ����ſ�������Խ��Խ�࣬��Ҷ�����������Լ���`ҵ���߼�`���µ�`���ݱ���`����ʱ�������ӵ��˼��ٸ�������֮��ĸ�ֵ˳����������ϵ��ʼ��ø��ӣ�һЩ�����߼��������������е����̽ṹ���׳Ʒ��ߣ�������ϵͳ������⡣û��һ������˵�����Щ����֮���������ϵ�Լ���Щҵ���߼�֮���ִ��˳��ÿ���µĿ��������ı�����ÿ���Ų����ⶼ��ʱ������

���ڶ��˲��룬�����ܼ���ϵͳ����θ��������֯�����͵����ݣ���θ��õ�ʵ�ַ��ӵ�ҵ���߼��������Ϊ��Ҫ`gparallel`������.


> `gparallel`��һ�����`DAG(Directed acyclic graph)`�Ĳ���֧���Զ������Ƶ���������ȿ�ܡ�

`DAG`�ڼ�����������Ź㷺��Ӧ�ã������ڴ����ݼ����п���ʹ��DAGָ��Hadoop�����ִ��˳��ȵȡ�����������Ҳ���㷺Ӧ�ã���Դ������DAG-based���ȿ��Ҳ�кܶ࣬����[cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow)�����ǣ��������п�ܶ������� **�ֹ����õķ�ʽ** ���ɵ���DAG��������һ��ӵ���ĸ�����ĵ���ϵͳ�У�`cpp-taskflow`��Ҫͨ�����淽ʽ������DAG��

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

�����������Ҫ����Ԥ�ڵ�DAGͼ��Ҫ�˹���ʽ����ÿ�����ڵ�֮���������ϵ�����ַ�ʽ��Ȼ���Ƚ�ֱ�ۣ�����ȱ��Ҳ�ǳ����ԣ�

* **���д��������ʱ���˹�����DAGͼ�Ƚ����Ѳ������׳�����ʵ�е�ҵ��ϵͳһ���Ƕ���ͬʱ�����������Ҫģ��owner�����е�����ڵ�֮���������ϵ�����˹�������ά���Խϲ**
* **��ҵ�����кܶ�ҵ�������������������ķ�ʽ������������������Ҫ���Ѵ���ʱ������ϵͳ�߼�������������ʾǿ��ת��Ϊ����������ʾ���������ϵͳ����ʱ������**

# gparallel��ν������

gparallel����Ҫ˼����3����

* `���ݻ���`�����������ݳ�Ա������**ҵ���߼�**��**����״̬**����Ϊ��ͬ�ļ��ϡ�
* `�����Ƶ�`�������еĴ����߼������չ��ܻ���Ϊ��ͬ��task�ڵ㣬�����Զ��Ƶ�task�ڵ�֮���������ϵ������DAG��
* `�������`��ͨ���������򣬽�`DAG`ת��Ϊ`ƫ��`��ʾ����ʹ��thread����coroutine��task���е��ȡ�

## ���ݻ���

�ڼ���ϵͳ�У�`����`һ�����ָ���������������洢�м��������ս���ı���������洢����`std::list`���洢�����`std::string`��gparallel��Ҫ��2���ǶȽ��л��֣�

* `����ҵ���߼�`���Ѳ�ͬҵ���߼�����Ҫ�õ������ݻ���Ϊ��ͬ�ļ��ϡ������в�ͬ�Ĺ��Ӫ����Ʒ�����Զ����Լ���User��Plan��Ad�ļ��ϣ��Լ�һЩ�洢���м����ı�����Ϊ�˷��������������ô�д��ĸ����ʾ����ҵ���߼����ֳ��������ݼ��ɣ�����`A`��`B`��`C`�ȵȡ�

* `��������״̬`������һ�������ڷ�����ͬһ��ҵ������ݼ���(A)���ڲ�ͬ�Ľ׶Σ��ֿ��Ի���Ϊ��ͬ��״̬������һ�������У���ʼ״̬�ǿ�(empty)����������Ժ���N�����(inited)���־�����һ�ΰ���CTR������(ranked)���������һ�νض�(truncate)����ô��Ӧ�ĸ�״̬`A_empty`��`A_inited`��`A_ranked`��`A_truncated`��

Ϊ�˷�����⣬���ǰѻ��ֳ�����ÿ���Ӽ�������`meta`��ǰ���ᵽ��`A_empty`��`A_inited`��`A_ranked`��`A_truncate`����`meta`����gparallel�У�������[`DECL_META`](include/meta.h)��������һ��`meta`��

> meta����ʾ**ָ��ҵ��**����Ҫ���������ݵļ��ϵ�**���ݽṹ**��

���gparallel�����ݵ�2��ݽ����ַ�ʽ�ǳ���Ҫ����Ϊgparallel��DAG�Զ��Ƶ��������������ڲ�ͬ��meta��

��ʵ�е�ϵͳ�У����ݳ�Աһ�������һ������`context`����`thread_data`�Ľṹ���С�����˼�壬��Щ���ݵ����÷�Χ����һ������һ���Ƚϳ����������һ���������̳߳��е�һ���߳������������������󼶱�����ݣ�����Ҳ���̼߳�������ݡ����`context`����`thread_data`�����ͣ����Ƕ���Ϊ`meta_storage_t`������һ�μ������õ����������ݣ���ͳһ�洢�����

ͨ������`getter`��`setter`�������ȶ��Ӽ�Ԫ�ص�ָ�������������`getter`��`setter`�ʹ������`meta`�а���������ݳ�Ա���Ӽ�֮��Ҳ���Ի��������ԭ������������е�`�̳�`��һ���ġ�ͬ�����һ������������һ��`meta`����Ҳͬ�����������`meta`�ĸ�`meta`��`�̳�`���Ƶ���ҪĿ����Ϊ�˱����ظ����弯�ϵ�Ԫ�أ����Ӵ���Ŀ�ά���ԡ�ͨ����������ӿ������`meta_storage_t`��`meta`��`�̳�`�Ĺ�ϵ��

<div><img align="center" width="75%" src="./image/meta.png"></div>

ͨ����ͼ���Կ�����`meta_common`������`thread_data::id`��`meta_a`ͬʱ������`thread_data::business_a`��`meta_common`������Ԫ�ء�`meta_b`ͬʱ������`thread_data::business_b`��`meta_common`������Ԫ�ء�

## ������

��`gparallel`�У�ʹ��һ��������ʾһ����������񣬺����Ĳ�����ʾ�����`����`��`���`���κ�һ��`meta`�ȿ�����Ϊ`����`��Ҳ������Ϊ`���`����������2��ģ���װ��`input`��`output`�����`meta`��`input`��װ�����������Ὣ�䵱��һ���������ݣ�ͬ�������`output`��װ����ᵱ�������

��ʾ����ĺ��������붨��Ϊ��ľ�̬��Ա�������������ֱ���Ϊ`process`����������Ϊ`void`�����磺

```cpp
struct DemoNode {
    void process(input<meta_a> a, input<meta_b> b, output<meta_c> c) {
        // process code
        c->mutable_business_c() = a->get_business_a() + b->get_business_b();
    }
}
```
�����`DemoNode`ʵ���˽�`business_a`��`business_b`�ĺ͸�ֵ��`business_c`���߼���
`process`��������ӵ��������`����`��������`���`������û��`����`�Ľڵ���Ϊ��ʼ�ڵ�֮һ��û��`���`�Ľڵ���Ϊ��ֹ�ڵ�֮һ��

## �������

ͨ��`register_node`���������нڵ�ע�ᵽ`DAG`�к�ʹ��`setup_dag_schema`�������Զ�ʵ�ֽڵ����������Լ�`DAG`���ɡ�������ɵ�`DAG`����`��������`�����ܵõ��������˳�򡣺���ɸ��ݾ��������ʵ�ֶ��̻߳��߶�Э�̵��ȡ�

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

# gparallelʵս

�ڱ�С���У����һ����ʵ�ĳ���������`gparallel`��ʹ���߼���

> ������������һ��ָ���Ĺ����У��ֱ�������`CTR`(�����)ֵ��`CPM`(ǧ��չʾ�ɱ�)����䵽���Ķ�Ӧ�ֶΣ����ֱ���`CTR`��`CPM`�������������2���µĹ����й�����ʹ�á�

���������ǹ�����ϵͳ����һ���Ƚϵ��͵��߼��������Ĵ�����[./demo/advprocess.cpp](./demo/advprocess.cpp)��

## ���ݻ���

������������һ������Ҫ�õ������ݶ���

�������� | ���� |  ����  
-|-|-
advs_original|  advlist_t |  ԭʼ�Ĺ����� |
ctr_data |  advlist_t | ģ�ͷ��ص�ctr����|
cpm_data |  advlist_t |  ģ�ͷ��ص�cpm����|
advs_ctr_ordered | ctr_response_t |  �����ctr����Ĺ�����|
advs_cpm_ordered | cpm_response_t |  �����cpm����Ĺ�����|

��������Ķ��壬���Ƕ������ݼ��ϣ�ҵ��ִ����Ҫ�õ����������ݶ�����`thread_data`��������У�

![demo_thread_data.png](./image/demo_thread_data.png)

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

���������Ǹ��ݲ�ͬ��ҵ�񣬽����ϻ���Ϊ��ͬ���Ӽ���ÿ���Ӽ�����һ��`meta`��һ��Ԫ�ؿ���ͬʱ���ڶ��`meta`��`meta`��`meta`��֮����Ի��������
������������������ǿ��Ժ������ܽ��5�������̣�ÿ�������̶���Ӧһ�����ݴ���ڵ㣺 
<table width="60%">
    <thead>
        <tr>
            <th>����</th><th>�ڵ�����</th><th>����</th><th>���</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>��ȡCTRֵ</td><td>get_ctr_node</td><td>advs_original</td><td>ctr_data</td>
        </tr>
        <tr>
            <td>��ȡCPMֵ</td><td>get_cpm_node</td><td>advs_original</td><td>cpm_data</td>
        </tr>
        <tr>
            <td>����ֶ�</td><td>fill_node</td>
            <td>advs_original <br>ctr_data <br>cpm_data</td>
            <td>advs_original*</td>
        </tr>
        <tr>
            <td>����CTR<br>�������</td><td>gen_ctr_node</td><td>advs_original*</td>
            <td>advs_ctr_ordered</td>
        </tr>
        <tr>
            <td>����CPM<br>�������</td><td>gen_cpm_node</td>
            <td>advs_original*</td><td>advs_cpm_ordered</td>
        </tr>
    </tbody>
</table>


���Կ���ԭʼ����Ĺ�������`advs_original`���������ǽ����װΪmeta`original`��

`get_ctr_node`�ڵ��`get_cpm_node`�ڵ�ͨ��meta`original`�ֱ��ȡ`ctr_data`��`cpm_data`�������ݣ�����������������meta`ctr`��meta`cpm`����װ��

`fill_node`�ڵ�Թ����н���������䣬����ע�⣬�ڵ����������meta`original`���������meta`original_with_ctr_cpm`����2��meta��ʵ�����϶���`advs_original`�ķ�װ��������Ϊ����2���׶�(�����ǰ������)�����Էֱ���2����ͬ��meta����ʾ����ʵ���ϣ����ǿ���ֱ��ʹ�ü̳й��ܣ�����meta`original`��

`gen_ctr_node`��`gen_cpm_node`�����붼����meta`original_with_ctr_cpm`����ʾ�������������`advs_original`���������ǰ��

�����Ϳ��Ը��ݲ�ͬ��ҵ���߼�����`thread_data`���ϻ���Ϊ��ͬ��`meta`��ÿ��ҵ��ֻ��Ҫ��ע�Ӽ���Ҫ�õ���Щ`meta`��������Щ`meta`���ɡ�
![demo_meta.png](./image/demo_meta.png)

## ������

����ʵ��`get_ctr_node`��`get_cpm_node`����������ڵ㡣
```cpp
struct get_ctr_node {
    static void process(input<original> ori, output<ctr> ctr) {
        INFO("[gparallel] get_ctr_node", "");
        ctr->mutable_ctr_data().resize(ori->mutable_advs_original().size());
        for (int pos = 0; pos < ori->mutable_advs_original().size(); pos++) {
            auto & adv = ori->mutable_advs_original()[pos];
            ctr->mutable_ctr_data()[pos] = 0.1 * static_cast<double>(adv.id);
        }
    }
};

struct get_cpm_node {
    static void process(input<original> ori, output<cpm> cpm) {
        INFO("[gparallel] get_cpm_node", "");
        cpm->mutable_cpm_data().resize(ori->mutable_advs_original().size());
        for (int pos = 0; pos < ori->mutable_advs_original().size(); pos++) {
            auto & adv = ori->mutable_advs_original()[pos];
            cpm->mutable_cpm_data()[pos] = 100.2 * static_cast<double>(adv.id);
        }

    }
};
```
����������Ĺ������ƣ����Ǹ���`original`�д洢�����й�棬��ȡ��Ӧ��`�����`��`ǧ��չʾ�ɱ�`���ݣ����ҷֱ𱣴浽`ctr`��`cpm`������meta������Ϊ�˼򻯣�����ȡһЩ�����������Ϊ`ctr`��Ϊ`cpm`����ʵ��������Ҫͬ�������첽����ģ�ͷ���������ȡ��

������ʵ��`fill_node`��
```cpp
struct fill_node {
    static void process(input<ctr> ctr, input<cpm> cpm, input<original> ori, 
        output<original_with_ctr_cpm> ori_ctr_cpm) {
        INFO("[gparallel] fill_node", "");
        for (int pos = 0; pos < ori->mutable_advs_original().size(); pos++) {
            auto & adv = ori_ctr_cpm->mutable_advs_original()[pos];
            adv.ctr = ctr->mutable_ctr_data()[pos];
            adv.cpm = cpm->mutable_cpm_data()[pos];
            INFO("[gparallel] ori_ctr_cpm adv:[%d] ctr:[%lf] cpm:[%lf]", adv.id, adv.ctr, adv.cpm);
        }
    }
};
```
`fill_node`�ڵ㽫ǰ��`get_ctr_node`��`get_cpm_node`�������ڵ�������Ϊ�Լ������룬�������`original_with_ctr_cpm`��������Կ�����`original`��`original_with_ctr_cpm`�ֱ���Ϊ�ڵ��������������Ȼ������meta�а��������ݶ���`advs_original`�����Ƕ�Ӧ��**��ͬ��״̬**��`original`������`advs_original`һ��ʼ��״̬����`original_with_ctr_cpm`�������Ѿ������`ctr`��Ϊ`cpm`���ݵ�״̬��

���ʵ��`gen_ctr_node`��`gen_cpm_node`��
```cpp
struct gen_ctr_node {
    static void process(input<original_with_ctr_cpm> ori_ctr_cpm, 
        output<ctr_ordered_advlist> ctr_ordered) {
        INFO("[gparallel] gen_ctr_node", "");
        ctr_ordered->mutable_advs_ctr_ordered() = ori_ctr_cpm->mutable_advs_original();
        std::sort(ctr_ordered->mutable_advs_ctr_ordered().begin(),
            ctr_ordered->mutable_advs_ctr_ordered().end(), 
            [](const auto& a, const auto& b)->bool{return a.ctr > b.ctr;});

    }
};

struct gen_cpm_node {
    static void process(input<original_with_ctr_cpm> ori_ctr_cpm, 
        output<cpm_ordered_advlist> cpm_ordered) {
        INFO("[gparallel] gen_cpm_node", "");
        cpm_ordered->mutable_advs_cpm_ordered() = ori_ctr_cpm->mutable_advs_original();
        std::sort(cpm_ordered->mutable_advs_cpm_ordered().begin(),
            cpm_ordered->mutable_advs_cpm_ordered().end(),
            [](const auto& a, const auto& b)->bool{return a.cpm > b.cpm;});
    }
};
```
������Ҫע����ǣ�`gen_ctr_node`��`gen_cpm_node`�������ڵ㣬��Ҫ����**�Ѿ������`ctr`��Ϊ`cpm`����**��`advs_original`����������meta����Ϊ`original_with_ctr_cpm`����һ�����������ȷ��DAG��˵�ǳ���Ҫ��`gparallel`����ͨ������ͬһ���ݵĲ�ͬ�׶Σ���ʵ����ȷ��������ȡ�

## �������

�����е�meta��node��������Ժ����ǿ���ͨ���������̽���������ȡ�
����������Ҫ������Ӧ���ڴ����洢�������е����ݱ�����������������һ��`thread_data`��ʵ����

```cpp
thread_data td{{advertisement(1), advertisement(2), advertisement(3)}, {},{},{},{}};
```

���ǻ���Ҫһ��`�ڵ�����`����������е����ݴ���ڵ㣺

```cpp
dag_schema<thread_data> nodes;
```

���������ǽ����е�`�ڵ�`��ע�ᵽ�ղ������`�ڵ�����`�С�

```cpp
register_node<thread_data, get_ctr_node, get_cpm_node>::reg(nodes);
register_node<thread_data, fill_node>::reg(nodes);
register_node<thread_data, gen_ctr_node, gen_cpm_node, end_node>::reg(nodes);
```

`register_node`ģ�渺��ע��ڵ㵽����`nodes`�У����е�һ��ģ����������������������մ洢���࣬Ҳ����`meta_storage_t`��ʣ��������ģ������ֱ�Ϊ����node��`reg`�������������Ϊ`dag_schema`���͵Ľڵ�����ʵ����

�������Ƶ����нڵ��������ϵ��

```cpp
setup_dag_schema<thread_data>(nodes);
```

������Ƕ�DAG��������нڵ����`��������`�����Ұ���������˳�����˽��е��ã�

```cpp
if (auto tasks = topological_sort<thread_data>(nodes); tasks) {
    for (auto task : tasks.value()) {
        INFO("Execute[%s]", task->name().c_str());
        task->mutable_executor()(&td);
    }
}
```



