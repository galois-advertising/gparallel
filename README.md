# gParallel <img align="right" width="30%" src="./image/logo.png">

[![Build Status](https://www.travis-ci.org/galois-advertising/gparallel.svg?branch=master)](https://www.travis-ci.org/galois-advertising/gparallel)


gparallel��һ������߼��Ƚϸ��ӵ���Ϣ����ϵͳ����ƵĲ���������ȿ�ܡ�����DAG(Directed acyclic graph)���в���������ȣ����Զ�������������������Ƶ�������ϵ��

# Quick start

* ��������
    - g++8
    - boost-v1.70
    - gtest-v1.10.0

���ز�����test

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

��gparallel��Ϊ����Ŀ��һ����

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

���ڵ�����ҵ��ϵͳ����ϵͳ�������ڣ�ϵͳҵ�񻹱Ƚϼ򵥣�ÿ��`��������`����ʱ��Ҫִ�е�ҵ���߼�Ҳ�Ƚϵ�һ����ʱ���󼶱��`���ݱ���`�Ƚ��٣���Щ�����ĸ�ֵ˳����������ϵҲһĿ��Ȼ���������ݼ�����һ����ȫ���Ե�һ�����̣�û���κ�`�첽����`��

���ǣ����ſ�������Խ��Խ�࣬��Ҷ�����������Լ���`ҵ���߼�`���µ�`���ݱ���`����ʱ�������ӵ��˼�ʮ��������֮��ĸ�ֵ˳����������ϵ��ʼ��ø��ӣ�һЩ�����߼��Ѿ�����ô����⣬�ڴ�ͬʱ��Ϊ������`�ⲿ����`���ã��������г����˴���`ͬ���ȴ�`��ϵͳ`ƽ��`��ʼ����������������������Ĺ�����ǿ��Ҫ��CI��Աһ��Ҫ����㹻��ע���Ա����Ŀ�����Ա�ܹ����״��������֮�����е��߼���

�����º󣬴��ͻȻ�������󼶱��`���ݱ���`�Ѿ���Ｘ�ٸ�������`ҵ���߼�`Ҳ��ø��Ӹ��ӡ����ʱ�򼴱�����ע�ͣ�Ҳû��һ������˵�����Щ����֮���������ϵ�Լ���Щҵ���߼�֮���ִ��˳�򣬼�ֱ����һ�����顣

ÿ���µĿ��������ı�����ÿ���Ų����ⶼ��ʱ������

�ԣ�����Ǳ������ڹ�˾��ϵͳ��״��500�˲��뿪���������д��룬�켶��ĵ������¡���Ϊ��ӵ����߹��ϵͳ֮һ�������֯���ӵ�ҵ���߼�����δ�Ÿ����͵����ݣ������Ϊʲô��Ҫgparallel.

# gparallel��ν������

gparallel����Ҫ˼����3����

* `���ݻ���`�����������ݳ�Ա������ҵ���߼�������״̬������Ϊ��ͬ�ļ��ϡ�
* `�����Ƶ�`�������еĴ����߼������չ��ܻ���Ϊ��ͬ��task�������Զ��Ƶ�task֮���������ϵ������DAG����ͼ��
* `�첽����`��ͨ��thread����coroutine����task���е��ȡ�

## ���ݻ���

�ڼ���ϵͳ�У�`����`һ�����ָ���������������洢�м��������ս��������gparallel��Ҫ��2���Ƕ�ȥ���л��֣�

* `ҵ���߼�`���Ѳ�ͬҵ���߼�����Ҫ�õ������ݻ���Ϊ��ͬ�ļ��ϡ�
* `����״̬`���������ݵ�״̬������˵�׶Σ���ͬһ����������Ϊ��ͬ�����ݼ��ϡ�

����ҵ���߼����������ǱȽϳ����˼·��ÿ�����ݼ����ǻ�
����״̬������ָ��ͬһ���������������������������δ��ֵ������ֵΪA������ֵΪB�Ĺ����У�ʵ�ʻ����3�����ϡ�

## �����Ƶ�

## �첽����