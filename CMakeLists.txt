PROJECT(gparallel)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++2a -fno-strict-aliasing -Wno-register -Wno-pessimizing-move")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++2a -fno-strict-aliasing -Wno-register -Wno-pessimizing-move")
SET(CMAKE_BUILD_TYPE "Release")

FIND_PACKAGE(Boost REQUIRED COMPONENTS log)
IF(NOT Boost_FOUND)
    MESSAGE("Can not find Boost")
ENDIF()
MESSAGE(STATUS "Found Boost IncPath ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "Found Boost LibPath ${Boost_LIBRARY_DIRS}")
MESSAGE(STATUS "Found Boost Libraries ${Boost_LIBRARIES}")


ADD_DEFINITIONS(-DBOOST_SPIRIT_THREADSAFE)
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
ADD_DEFINITIONS(-D_DEBUG)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/common/util/include")

ENABLE_TESTING()
FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

IF (NOT TARGET common)
    ADD_SUBDIRECTORY(common)
ENDIF()


ADD_EXECUTABLE(gparallel_test
"${PROJECT_SOURCE_DIR}/test/main.cpp" 
"${PROJECT_SOURCE_DIR}/src/util.cpp" 
"${PROJECT_SOURCE_DIR}/test/test_invoke_ex.cpp" 
"${PROJECT_SOURCE_DIR}/test/test_data_meta.cpp" 
"${PROJECT_SOURCE_DIR}/test/test_type_id.cpp" 
"${PROJECT_SOURCE_DIR}/test/test_node.cpp" 
"${PROJECT_SOURCE_DIR}/test/test_register_node.cpp" 
"${PROJECT_SOURCE_DIR}/src/dag_schema.cpp" 
"${PROJECT_SOURCE_DIR}/src/dag_instance.cpp" 
"${PROJECT_SOURCE_DIR}/src/node_schema.cpp" 
"${PROJECT_SOURCE_DIR}/src/node_instance.cpp" 
"${PROJECT_SOURCE_DIR}/src/debug.cpp" 
"${PROJECT_SOURCE_DIR}/common/util/src/log.cpp" 
)
TARGET_LINK_LIBRARIES(gparallel_test ${GTEST_BOTH_LIBRARIES} common)