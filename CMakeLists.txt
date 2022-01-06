cmake_minimum_required(VERSION 3.16)
project(goose)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(goose main.cpp config.h index/parse.cpp index/parse.h index/merge.cpp index/merge.h index/list.cpp index/list.h compress/varbyte.cpp compress/varbyte.h compress/delta.cpp compress/delta.h util/aho_corasick.cpp util/aho_corasick.h util/aho_corasick.cpp util/aho_corasick.h util/inverted_list.cpp util/inverted_list.h util/lexicon.cpp util/lexicon.h util/doc_info.cpp util/doc_info.h query/search.cpp query/search.h query/snippet.cpp query/snippet.h util/bm25.cpp util/bm25.h util/radix_tree.cpp util/radix_tree.h)
