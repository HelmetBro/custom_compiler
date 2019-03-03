//
// Created by Eric Parsons on 2019-02-27.
//

#ifndef INC_241COMPILER_SSA_HPP
#define INC_241COMPILER_SSA_HPP

#include <string>
#include <unordered_map>

#include "../basic_block.hpp"

typedef std::unordered_map<basic_block*, std::vector<basic_block*>> map;

class SSA{

public:

    //every ssa pass update this variable
    static void ssa(basic_block * IR_start, std::unordered_map<std::string, int> version_table){
        v_table = &version_table;
        variable_versioning(IR_start);
    }

private:

    static std::vector<unsigned long> visited;
    static std::unordered_map<std::string, int> * v_table;

    //EXACT same traversal as networking switches!!! Freaking cool!!!
    static void variable_versioning(basic_block * block){

        if(std::find(visited.begin(), visited.end(), block->node_num) != visited.end())
            return;

        visited.push_back(block->node_num);
        version_assignments(block);

        //if it branches
        if(block->alternate != nullptr)
            variable_versioning(block->alternate);

        if(block->initial != nullptr && block != block->initial->mother)
            variable_versioning(block->initial);

    }

    static void version_assignments(basic_block * block){

//        for (auto &instruction : block->instructions)
//            instruction.arguments.begin()->var =
//                    version_it(instruction.arguments.begin()->var, instruction.operation);

        for(auto &e : block->instructions)
            if(e.arguments.begin()->type == argument::ARG_TYPE::VAR)
                e.arguments.begin()->var = version_it(e.arguments.begin()->var, e.operation);
    }

    static std::string version_it(std::string &var, IR_MNEMONIC op){

        //variable found is not in the global vars list!
        auto element = v_table->find(var);
        if(element == v_table->end())
            throw syntax_error();

        if(op == IR_MNEMONIC::MOVE)
            ++(*element).second;

        //(*element).second++) vs (element->second++). clion says latter "element->second" not used?
        return var.append(std::to_string((*element).second));
    }

};

std::unordered_map<std::string, int> * SSA::v_table = nullptr;
std::vector<unsigned long> SSA::visited;

#endif //INC_241COMPILER_SSA_HPP
