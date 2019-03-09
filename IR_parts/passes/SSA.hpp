//
// Created by Eric Parsons on 2019-02-27.
//

#ifndef INC_241COMPILER_SSA_HPP
#define INC_241COMPILER_SSA_HPP

#include <string>
#include <unordered_map>
#include <stack>

#include "../basic_block.hpp"

typedef std::unordered_map<basic_block*, std::vector<basic_block*>> map;
typedef std::unordered_map<std::string, int> table;

class phi_instruction {

public:

    unsigned long line_num;

    std::string base;

    std::string first;
    std::string second;

    phi_instruction(unsigned long ln, std::string ident, std::string first, std::string second){
        this->line_num = ln;
        this->base = ident;
        this->first = first;
        this->second = second;

    }

    instruction phi_to_instruction(){

        argument inst_base = argument(this->base, argument::ARG_TYPE::VAR);
        argument inst_first = argument(this->first, argument::ARG_TYPE::VAR);
        argument inst_second = argument(this->second, argument::ARG_TYPE::VAR);

        return instruction(line_num, IR_MNEMONIC::PHI, inst_base, inst_first, inst_second);

    }

};

struct join_block {

    std::vector<phi_instruction> phi_insts;

    /** Updates join block with variable */
    void construct_phi(instruction *single_inst, bool from_initial, table v_table){

        std::string arg = std::string(single_inst->arguments.begin()->var);

        /* Add a new phi for it. */

        auto element = v_table.find(arg); //finding current version within the table
        if(element == v_table.end())
            throw syntax_error();

        std::string new_arg(single_inst->arguments.begin()->var); //copy variable and append the current versioning scheme
        new_arg.append("_" + std::to_string((*element).second));

        phi_insts.emplace_back(phi_instruction( //create new phi and add it to the "joins block"
                ++instruction::instruction_counter,
                arg,
                new_arg,
                new_arg
                ));
    }

    void init_phi_args(bool from_initial, table v_table){

        //loop over phi functions
        for(auto &p : phi_insts){

            auto element = v_table.find(p.base);
            if(element == v_table.end())
                throw syntax_error();

            std::string arg(p.base);
            arg.append("_" + std::to_string((*element).second));

            if(from_initial)
                p.first = arg;
            else
                p.second = arg;

        }

    }

};

class SSA{

public:

    //every ssa pass update this variable
    static void ssa(basic_block * IR_start, std::unordered_map<std::string, int> version_table){
        variable_version_table = version_table;

        visited.clear();
        auto join_stack = std::stack<join_block *>();
        phi_versioning(IR_start, join_stack, true, variable_version_table);
    }

private:

    static std::vector<unsigned long> visited;
    static table variable_version_table;

    static void construct_phis(basic_block *block, std::stack<join_block *> &joins, bool from_initial){

        if(joins.empty())
            return;

        join_block * join = joins.top();

        for(auto &inst : block->instructions)
            if(inst.operation == IR_MNEMONIC::MOVE)
                join->construct_phi(&inst, from_initial, variable_version_table);
    }

    static void init_phi_args(basic_block *block, std::stack<join_block *> &joins, bool from_initial){

        if(joins.empty())
            return;

        join_block * join = joins.top();
        join->init_phi_args(from_initial, variable_version_table);
    }

    //EXACT same traversal as networking switches!!! Freaking cool!!!
    static void phi_versioning(basic_block *block, std::stack<join_block *> &joins, bool from_initial, table v_table){

        if(std::find(visited.begin(), visited.end(), block->node_num) != visited.end())
            return;
        visited.push_back(block->node_num);

        if(parents_been_visited(block)){
            v_table = variable_version_table; //update for the new variable table
            add_phis_to_block(block, joins);
        }

        //if we were branched into, then start the phi process
        construct_phis(block, joins, from_initial);
//        join_block * top = joins.empty() ? nullptr : joins.top(); // GET RID
        version_assignments(block, v_table);
        version_phis(block);

        update_global_var_table(v_table);

        init_phi_args(block, joins, from_initial);

        if(block->initial != nullptr && //if i have a child
            (block->initial->mother == nullptr || //and its an only child. else, i must be the mother to recurse
                (block->initial->mother != nullptr && block == block->initial->mother))){

            if(block->alternate != nullptr)
                joins.push(new join_block());

            phi_versioning(block->initial, joins, true, v_table);
        }


        if(block->alternate != nullptr){
            phi_versioning(block->alternate, joins, false, v_table);
        }

    }

    /** Iterate through entries and find updated values. */
    static void update_global_var_table(const table &v_table){

        for(auto &e : v_table){

            int & global_v = variable_version_table.at(e.first);

            if(global_v < e.second)
                global_v = e.second;
        }

    }

    static void add_phis_to_block(basic_block *block, std::stack<join_block *> &joins){
        join_block * top = joins.top();
        joins.pop();

        for(auto &phi : top->phi_insts)
            block->instructions.insert(block->instructions.begin(), phi.phi_to_instruction());
    }

    static bool parents_been_visited(basic_block *block){
        return block->father != nullptr && block->mother != nullptr;
    }

    static void version_assignments(basic_block * block, table & v_table){

        //iterate through block instructions
        for(auto &e : block->instructions)

                //iterate through all of its arguments
                for(int i = 0; i < e.arguments.size(); i++){

                    //if we're a phi, only version the first argument
                    if(e.operation == IR_MNEMONIC::PHI)
                        continue;

                    //if we're a variable
                    if(e.arguments[i].type == argument::ARG_TYPE::VAR)

                        //then version it!
                        version_arg(e.arguments[i].var, e.operation, i == 0, v_table);
                }

    }
    static void version_arg(std::string &var, IR_MNEMONIC op, bool first_arg, table & v_table){

        //variable found is not in the global vars list!
        auto element = v_table.find(var);
        if(element == v_table.end())
            throw syntax_error();

        if(first_arg && op == IR_MNEMONIC::MOVE) //increase version number if we're a MOVE
            ++(*element).second;

        var.append("_" + std::to_string((*element).second));
    }

    static void version_phis(basic_block * block){

        //iterate through block instructions
        for(auto &e : block->instructions)
            if(e.operation == IR_MNEMONIC::PHI){
                auto element = variable_version_table.find(e.arguments[0].var);
                if(element == variable_version_table.end())
                    throw syntax_error();

                e.arguments[0].var.append("_" + std::to_string(++(*element).second));
            }

    }

};

table SSA::variable_version_table;
std::vector<unsigned long> SSA::visited;

#endif //INC_241COMPILER_SSA_HPP
