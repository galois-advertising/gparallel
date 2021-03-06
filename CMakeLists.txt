PROJECT(gparallel)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++2a -fno-strict-aliasing -Wno-register -Wno-pessimizing-move")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++2a -fno-strict-aliasing -Wno-register -Wno-pessimizing-move")
SET(CMAKE_BUILD_TYPE "Release")

# boost
FIND_PACKAGE(Boost REQUIRED COMPONENTS log)
IF(NOT Boost_FOUND)
    MESSAGE("Can not find Boost")
ENDIF()

ADD_DEFINITIONS(-DBOOST_SPIRIT_THREADSAFE)
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
ADD_DEFINITIONS(-D_DEBUG)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# gtest
IF (NOT TARGET gtest)
    ADD_SUBDIRECTORY(gtest)
    ENABLE_TESTING()
    INCLUDE_DIRECTORIES(SYSTEM
        ${gtest_SOURCE_DIR}
        ${gtest_SOURCE_DIR}/include)
ENDIF()

# common
IF (NOT TARGET common)
    ADD_SUBDIRECTORY(common)
ENDIF()

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/common/util/include")

# libgparallel.a
file(GLOB gparallel_srcs "${PROJECT_SOURCE_DIR}/src/*.cpp")
ADD_LIBRARY(gparallel STATIC ${gparallel_srcs})

# gparallel_test
file(GLOB test_srcs "${PROJECT_SOURCE_DIR}/test/*.cpp")
ADD_EXECUTABLE(test ${test_srcs})
TARGET_LINK_LIBRARIES(test gtest common gparallel)

# demo
file(GLOB demo_srcs "${PROJECT_SOURCE_DIR}/demo/*.cpp")
ADD_EXECUTABLE(demo ${demo_srcs})
TARGET_LINK_LIBRARIES(demo gtest common gparallel)
