cmake_minimum_required(VERSION 3.12)
project(241Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(241Compiler main.cpp token.hpp lex_analyzer.hpp TYPE.hpp tables/symbol_table.hpp tables/keyword_table.hpp)