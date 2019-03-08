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

class join_block;
class phi_instruction : instruction{

public:

    unsigned long line_num;

    argument * base = nullptr; //"new" phi value

    argument * first = nullptr;
    argument * second = nullptr;

    //variable name without version
    std::string ident;

//    phi_instruction(unsigned long ln, argument * initial){
//        this->line_num = ln;
//        this->base = initial;
//        this->first = initial;
//        this->second = initial;
//
//        //var ident
//        ident = initial->var.substr(0, initial->var.find('_'));
//    }

    phi_instruction(unsigned long ln, std::string ident, argument * first, argument * second){
        this->line_num = ln;
        this->base = first;
        this->first = first;
        this->second = second;

        //var ident
        this->ident = ident;//initial->var.substr(0, initial->var.find('_'));
    }

//    ~phi_instruction(){
//        delete first;
//        delete second;
//    }

};

struct join_block {

    std::vector<phi_instruction> phi_insts;

    void combine(join_block * join, bool from_initial){

        //check and see if we already have the phi. If so, just update it. Else, create it.
        for(auto &j : join->phi_insts){

            //if we already have a phi with that variable
            auto phi = has_a_phi_for_variable(j.ident);
            if(phi == nullptr){
                phi_insts.emplace_back(j);
                phi = has_a_phi_for_variable(j.ident);
            }

            if (from_initial)
                phi->first = j.base;
            else
                phi->second = j.base;
        }
    }

    phi_instruction * has_a_phi_for_variable(const std::string &ident){
        for(auto &phi : this->phi_insts){
            if(phi.ident == ident)
                return &phi;
        }
        return nullptr;
    }

    /** Updates join block with variable */
    void make_phis(instruction *single_inst, bool from_initial, std::unordered_map<std::string, int> *v_table){

        //gets rid of versioning
        std::string arg = std::string(single_inst->arguments.begin()->var);

        //loop over phi functions to find the
        for(auto &p : phi_insts)
            if(p.ident == arg){

                //we found the phi! lets update it
                if(from_initial)
                    p.first = &single_inst->arguments.front();
                else
                    p.second = &single_inst->arguments.front();

                return;
            }

        /* If we can't find it, add a new phi for it. That said, we need to first
         * add the current version of the variable. Then, add the new version
         * to the phi, in which it's argument placement depends on whether it
         * came from the alternate or initial branch. */

        auto element = v_table->find(arg); //finding current version within the table
        if(element == v_table->end())
            throw syntax_error();

        std::string temp(single_inst->arguments.begin()->var); //copy variable and append the current versioning scheme
        temp.append("_" + std::to_string((*element).second));

        auto first_argument = new argument(*single_inst->arguments.begin());
        first_argument->var.append("_" + std::to_string(((*element).second) + 1));

        auto first = from_initial ? first_argument : new argument(temp, argument::ARG_TYPE::VAR);
        auto second = from_initial ? new argument(temp, argument::ARG_TYPE::VAR) : first_argument;

        phi_insts.emplace_back(phi_instruction( //create new phi and add it to the "joins block"
                ++instruction::instruction_counter,
                arg,
                first,
                second
                ));
    }
};

class SSA{

public:

    //every ssa pass update this variable
    static void ssa(basic_block * IR_start, std::unordered_map<std::string, int> version_table){
        v_table = &version_table;

        visited.clear();
        auto join_stack = std::stack<join_block *>();
        phi_versioning(IR_start, join_stack, true);
    }

private:

    static std::vector<unsigned long> visited;
    static std::unordered_map<std::string, int> * v_table;

    static void update_phi(basic_block * block, std::stack<join_block *> &joins, bool from_initial){
        join_block * join = joins.top();
        for(auto &inst : block->instructions){
            if(inst.operation == IR_MNEMONIC::MOVE)
                join->make_phis(&inst, from_initial, v_table);
        }//for loop
    }

    //EXACT same traversal as networking switches!!! Freaking cool!!!
    static void phi_versioning(basic_block *block, std::stack<join_block *> &joins, bool from_initial){

        if(std::find(visited.begin(), visited.end(), block->node_num) != visited.end())
            return;
        visited.push_back(block->node_num);

        //if my parents have been visited, combine join block with body block
        if(parents_been_visited(block->father, block->mother))
            body_plus_join(block, joins);

        if(!joins.empty())
            update_phi(block, joins, from_initial);

        version_assignments(block);

        //if it branches
        if(block->alternate != nullptr){
            joins.push(new join_block());

            if(block->alternate->mother != block)
                phi_versioning(block->alternate, joins, false);
        }

        if(block->initial != nullptr && block != block->initial->mother)
            phi_versioning(block->initial, joins, true);

    }

    static void body_plus_join(basic_block *block, std::stack<join_block *> &joins){

        auto top = joins.top();
        joins.pop();

        for(auto &j : top->phi_insts){
            std::string copy = j.ident;
            j.base = new argument(version_it(copy, IR_MNEMONIC::PHI, true), argument::ARG_TYPE::VAR);//copy;

            auto new_inst = new instruction(j.line_num, IR_MNEMONIC::PHI, j.base, j.first, j.second);
            block->instructions.insert(block->instructions.begin(), *new_inst);
        }

        std::cout << "joined: " << block->father->node_num << " and " << block->mother->node_num << std::endl;

        //merging previous phi with new phi's
        if(!joins.empty()){
            join_block * new_top = joins.top();
            new_top->combine(top, true);
        }

    }

    static bool parents_been_visited(basic_block * father, basic_block * mother){
        return father != nullptr && mother != nullptr;
    }
    static void version_assignments(basic_block * block){
        for(auto &e : block->instructions)
            for(int i = 0; i < e.arguments.size(); i++){
                argument * arg = &e.arguments[i];
                if(arg->type == argument::ARG_TYPE::VAR && e.operation != IR_MNEMONIC::PHI)
                    version_it(arg->var, e.operation, i == 0);
            }

    }
    static std::string version_it(std::string &var, IR_MNEMONIC op, bool first_arg){

        //variable found is not in the global vars list!
        auto element = v_table->find(var);
        if(element == v_table->end())
            throw syntax_error();

        if((op == IR_MNEMONIC::MOVE || op == IR_MNEMONIC::PHI) && first_arg)
            ++(*element).second;

        //(*element).second++) vs (element->second++). clion says latter "element->second" not used?
        return var.append("_" + std::to_string((*element).second));
    }

};

std::unordered_map<std::string, int> * SSA::v_table = nullptr;
std::vector<unsigned long> SSA::visited;

#endif //INC_241COMPILER_SSA_HPP
