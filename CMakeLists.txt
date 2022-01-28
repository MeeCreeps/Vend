cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_CXX_STANDARD 14)
project(Vend CXX)

set(VEND_SRC_INCLUDE ${PROJECT_SOURCE_DIR}/src/include ${PROJECT_SOURCE_DIR}/src/* ${PROJECT_SOURCE_DIR}/src/include/thirdpart)
add_subdirectory(./src/include/thirdpart/googletest)
set(GTEST
        gtest
        pthread)

# db library
file(GLOB DB_ENGINE_SRC ${PROJECT_SOURCE_DIR}/src/dbengine/*.cpp ${PROJECT_SOURCE_DIR}/src/include/dbengine/*.h)
add_library(DB_ENGINE STATIC ${DB_ENGINE_SRC} )
target_link_libraries(DB_ENGINE librocksdb.a -lpthread -lsnappy -lbz2 -lzstd -ldl libz.so)

include_directories(${VEND_SRC_INCLUDE})
file(GLOB_RECURSE ALL_SRCS "*.h" "*.cpp")


add_executable(Vend main.cpp )
add_executable(TEST test.cpp)
FIND_PACKAGE(OpenMP REQUIRED)

#openmp
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()

#link_directories(${PROJECT_SOURCE_DIR}/include/lib ${PLATFORM_LIBS})


add_library(SRCS STATIC ${ALL_SRCS} test.cpp)
target_link_libraries(Vend SRCS DB_ENGINE)
target_link_libraries(TEST SRCS DB_ENGINE)
target_link_libraries(Vend ${GTEST})