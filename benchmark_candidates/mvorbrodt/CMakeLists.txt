cmake_minimum_required(VERSION 3.0.0)

project(blog VERSION 0.1
        DESCRIPTION "Code samples from https://vorbrodt.blog"
        LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")

add_executable(pool pool_test.cpp)
add_executable(queue queue.cpp)
