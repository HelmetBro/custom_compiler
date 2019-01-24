cmake_minimum_required(VERSION 3.12)
project(241Compiler)

set(CMAKE_CXX_STANDARD 11)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(241Compiler main.cpp token.hpp lex_analyzer.hpp tables/symbol_table.hpp
        tables/keyword_table.hpp syntax_error_exception.hpp abstract_syntax_tree.hpp blocks/block.hpp
        blocks/main_block.hpp blocks/identifier_block.hpp blocks/function_block.hpp blocks/body_block.hpp
        blocks/constant_block.hpp blocks/statements/assignment.hpp blocks/statements/rela_pressions/term.hpp
        blocks/statements/rela_pressions/factor.hpp blocks/statements/rela_pressions/expression.hpp
        blocks/statements/rela_pressions/designator.hpp blocks/statements/function_call.hpp
        blocks/statements/if_statement.hpp blocks/statements/rela_pressions/relation.hpp
        blocks/statements/while_statement.hpp blocks/statements/return_statement.hpp blocks/statements/statement.hpp temp.hpp)
