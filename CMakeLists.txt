cmake_minimum_required(VERSION 3.20)
project(goose)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(goose main.cpp config.h index/parse.cpp index/parse.h index/merge.cpp index/merge.h index/list.cpp index/list.h compress/varbyte.cpp compress/varbyte.h compress/delta.cpp compress/delta.h)
