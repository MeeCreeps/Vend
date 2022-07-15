cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++17 -L/usr/local/lib -L/usr/local/lib64")
project(Vend CXX)

# probuf

FIND_PACKAGE(PkgConfig REQUIRED)				#PkgConfig名字是固定的，代表准备加入pkg-config模块，即查找/usr/bin/pkg-config
#eg: -- Found PkgConfig: /usr/bin/pkg-config (found version "0.29.1")
PKG_SEARCH_MODULE(PROTOBUF REQUIRED protobuf)	#通过执行pkg-config程序，并指定我需要的模块是opencv,注意opencv名字固定，是源于安装OpenCV生成的opencv.pc，PKG_OPENCV是前缀(观察下面)，
#eg: -- Checking for one of the modules 'opencv'
PKG_SEARCH_MODULE(GRPC REQUIRED grpc)
PKG_SEARCH_MODULE(GRPCPP REQUIRED grpc++)


set(PROTO_SRC_DIR ${PROJECT_SOURCE_DIR}/src/include/dbengine/grpc)
include_directories(${PROTO_SRC_DIR})

set(VEND_SRC_INCLUDE ${PROJECT_SOURCE_DIR}/src/include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/include/thirdpart)
add_subdirectory(./src/include/thirdpart/googletest)
set(GTEST
        gtest
        pthread)

# db library
file(GLOB DB_ENGINE_SRC ${PROJECT_SOURCE_DIR}/src/dbengine/*.cpp ${PROJECT_SOURCE_DIR}/src/include/dbengine/*.h
        ${PROJECT_SOURCE_DIR}/src/include/dbengine/grpc/*.h ${PROJECT_SOURCE_DIR}/src/dbengine/grpc/*.cc)
add_library(DB_ENGINE STATIC ${DB_ENGINE_SRC} src/include/dbengine/grpc/api.pb.cc src/include/dbengine/grpc/api.pb.cc)
target_link_libraries(DB_ENGINE librocksdb.a -lpthread -lsnappy -lbz2 -lzstd -ldl libz.so -llz4 -lneo4j-client -lssl
        -lcrypto -lm   -lgrpc++_reflection ${GRPCPP_LIBRARIES} ${GRPC_LIBRARIES} ${PROTOBUF_LIBRARIES})





include_directories(${VEND_SRC_INCLUDE})
file(GLOB_RECURSE ALL_SRCS "*.h" "*.cpp" ".cc")


#FIND_PACKAGE(OpenMP REQUIRED)

#openmp
#if (OPENMP_FOUND)
#    message("OPENMP FOUND")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#endif ()

#link_directories(${PROJECT_SOURCE_DIR}/include/lib ${PLATFORM_LIBS})

add_library(SRCS STATIC ${ALL_SRCS}  src/include/dbengine/grpc/api.pb.cc src/include/dbengine/grpc/api.grpc.pb.cc src/include/dbengine/grpc/api.grpc.pb.h src/include/dbengine/grpc/api.pb.h)
target_link_libraries(SRCS  ${GRPCPP_LIBRARIES} ${GRPC_LIBRARIES} ${PROTOBUF_LIBRARIES})

option(TEST_T "options for triangle" OFF)
option(TEST "options for intersection filter " OFF)
option(TEST_T1 "count triangles by vend" OFF)
option(TEST_T2 "vend filter adj " OFF)

if (TEST)
    add_definitions(-DVEND_LEVEL=-1)
endif (TEST)

if (TEST_T)
    add_definitions(-DVEND_LEVEL=0)
endif (TEST_T)


if (TEST_T1)
    add_definitions(-DVEND_LEVEL=1)
endif (TEST_T1)
if (TEST_T2)
    add_definitions(-DVEND_LEVEL=2)
endif (TEST_T2)
add_executable(Vend main.cpp ${PROTO_SRC_DIR})
add_executable(Triangle triangle_test.cpp)

target_link_libraries(Vend SRCS DB_ENGINE)
target_link_libraries(Vend ${GTEST})

target_link_libraries(Triangle SRCS DB_ENGINE)
add_executable(TEST test.cpp)
target_link_libraries(TEST SRCS DB_ENGINE)


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

