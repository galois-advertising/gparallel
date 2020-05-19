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

`gparallel`��һ�����`DAG(Directed acyclic graph)`��������ȿ�ܡ�`DAG`�ڼ�����������Ź㷺��Ӧ�ã������ڴ����ݼ����п���ʹ��DAGָ��Hadoop�����ִ��˳��ȵȡ�����������Ҳ���㷺Ӧ�ã���Դ������DAG-based���ȿ��Ҳ��ʤö�١��������м������п�ܶ����������õķ�ʽ���ɵ���DAGͼ������Ƚϳ�������[cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow)��������һ��ӵ���ĸ�����ĵ���ϵͳ�У�`cpp-taskflow`��Ҫͨ�����淽ʽ������DAG��

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

* `���ݻ���`�����������ݳ�Ա������ҵ���߼�������״̬������Ϊ��ͬ�ļ��ϡ�
* `�����Ƶ�`�������еĴ����߼������չ��ܻ���Ϊ��ͬ��task�������Զ��Ƶ�task֮���������ϵ������DAG����ͼ��
* `�������`��ͨ���������򣬽�`DAG`ת��Ϊ`ƫ��`��ʾ����ʹ��thread����coroutine��task���е��ȡ�

## ���ݻ���

�ڼ���ϵͳ�У�`����`һ�����ָ���������������洢�м��������ս���ı�����gparallel��Ҫ��2���Ƕ�ȥ���л��֣�

* `����ҵ���߼�`���Ѳ�ͬҵ���߼�����Ҫ�õ������ݻ���Ϊ��ͬ�ļ��ϡ������в�ͬ�Ĺ��Ӫ����Ʒ�����Զ����Լ���User��Plan��Ad�ļ��ϣ��Լ�һЩ�洢���м����ı�����Ϊ�˷��������������ô�д��ĸ����ʾ����ҵ���߼����ֳ��������ݼ��ɣ�����`A`��`B`��`C`�ȵȡ�

* `��������״̬`������һ�������ڷ�����ͬһ��ҵ������ݼ���(A)���ڲ�ͬ�Ľ׶Σ��ֿ��Ի���Ϊ��ͬ��״̬������һ�������У���ʼ״̬�ǿ�(empty)����������Ժ���N�����(inited)���־�����һ�ΰ���CTR������(ranked)���������һ�νض�(cut)����ô��Ӧ�ĸ�״̬`A_empty`��`A_inited`��`A_ranked`��`A_cut`��Ϊ�˷�����⣬���ǰѶ�������״̬�����ݽṹ������`meta`��ǰ���ᵽ��`A_empty`��`A_inited`��`A_ranked`��`A_cut`����`meta`����gparallel�У�������[`DECL_META`](include/meta.h)��������һ��`meta`��

���gparallel�����ݵ�2��ݽ����ַ�ʽ�ǳ���Ҫ����Ϊgparallel��DAG�Զ��Ƶ��������������ڲ�ͬ��meta��

��ʵ�е�ϵͳ�У����ݳ�Աһ�������һ������`context`����`thread_data`�Ľṹ���С�����˼�壬��Щ���ݵ����÷�Χ����һ������һ���Ƚϳ����������һ���������̳߳��е�һ���߳������������������󼶱�����ݣ�����Ҳ���̼߳�������ݡ����`context`����`thread_data`�����ͣ����Ƕ���Ϊ`meta_storage_t`��������meta�õ������ݣ���ͳһ�洢�����

`meta`��`meta_storage_t`�Ӽ���һ��ָ����ͨ������`getter`��`setter`��ʵ�ֵģ����������`getter`��`setter`�ʹ������`meta`�а���������ݳ�Ա���Ӽ�֮��Ҳ���Ի��������ԭ������������е�`�̳�`��һ���ġ�ͬ�����һ������������һ��`meta`����Ҳͬ�����������`meta`�ĸ�`meta`��`�̳�`���Ƶ���ҪĿ����Ϊ�˱����ظ����弯�ϵ�Ԫ�أ����Ӵ���Ŀ�ά���ԡ�ͨ����������ӿ������`meta_storage_t`��`meta`��`�̳�`�Ĺ�ϵ��

<div><img align="center" width="75%" src="./image/meta.png"></div>

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

������������һ������Ҫ�õ������ݶ���
�������� | ���� |  ����  
-|-|-
advs_original|  advlist_t |  ԭʼ�Ĺ����� |
ctr_data |  advlist_t | ģ�ͷ��ص�ctr����|
cpm_data |  advlist_t |  ģ�ͷ��ص�cpm����|
advs_ctr_ordered | ctr_response_t |  �����ctr����Ĺ�����|
advs_cpm_ordered | cpm_response_t |  �����cpm����Ĺ�����|

��������Ķ��壬���Ƕ���`meta_storage_t`��

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

������������������ǿ��Ժ������ܽ��5�������̣�ÿ�������̶���Ӧһ�����ݴ���ڵ㣺

ͬ��ÿ�������̵�������������Զ���Ϊ��
<center>
<table>
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
            <td>advs_original &amp; ctr_data &amp; cpm_data</td>
            <td>advs_original(filled)</td>
        </tr>
        <tr>
            <td>����CTR�������</td><td>gen_ctr_node</td><td>advs_original(filled)</td>
            <td>advs_ctr_ordered</td>
        </tr>
        <tr>
            <td>����CPM�������</td><td>gen_cpm_node</td>
            <td>advs_original(filled)</td><td>advs_cpm_ordered</td>
        </tr>
    </tbody>
</table>
<center>

<img align="right" width="50%" src="./image/metas.png">


