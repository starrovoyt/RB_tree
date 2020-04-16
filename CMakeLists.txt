cmake_minimum_required(VERSION 3.5)
project(RB_Tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(rb_tree
        src/main.cpp
        src/rbtree.h
        src/rbtree.hpp
)

add_executable(rb_tree_test
        src/rbtree.h
        src/rbtree.hpp
)
