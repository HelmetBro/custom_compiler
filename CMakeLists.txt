cmake_minimum_required(VERSION 3.12)
project(241Compiler)

set(CMAKE_CXX_STANDARD 11)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(241Compiler main.cpp token.hpp lex_analyzer.hpp tables/symbol_table.cpp tables/keyword_table.cpp syntax_error_exception.hpp)