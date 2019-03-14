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
    table v_table;
    bool from_initial = true;

    explicit join_block(table tab){
        v_table = std::move(tab);
    }

    /** Updates join block with variable */
    void construct_or_refurbish_phi(instruction *single_inst, table &v_table){

        std::string arg = std::string(single_inst->arguments.begin()->var);

        //if we already have a phi, then we need to update it
        if(contains_phi_for_variable(arg)){
            init_phi_args(v_table);
            return;
        }

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

    void init_phi_args(table & v_table){

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

    bool contains_phi_for_variable(std::string var){
        for(auto &p : phi_insts)
            if(p.base == var)
                return true;
        return false;
    }

//    void merge_phis(join_block * popped){
//
//        for(auto &p : popped->phi_insts){
//
//            if(contains_phi_for_variable(p.base)){
//
//
//                if(from_initial)
//                    p.first = arg;
//                else
//                    p.second = arg;
//
//
//            }
//        }
//
//
//        for(auto &p : phi_insts){
//
//
//
//
//            if(from_initial)
//                p.first = arg;
//            else
//                p.second = arg;
//
//        }
//
//    }

};

class SSA{

public:

    //every ssa pass update this variable
    static void ssa(basic_block * IR_start, std::unordered_map<std::string, int> version_table){
        variable_version_table = std::move(version_table);

        visited.clear();
        auto join_stack = std::stack<join_block *>();
        phi_versioning(IR_start, join_stack);
    }

private:

    static std::vector<unsigned long> visited;
    static table variable_version_table;
    static join_block * recently_popped;

    static void construct_phis(basic_block *block, std::stack<join_block *> &joins, table &v_table){

        if(joins.empty())
            return;

        join_block * top = joins.top();

        for(auto &inst : block->instructions)
            if(inst.operation == IR_MNEMONIC::MOVE || inst.operation == IR_MNEMONIC::PHI)
                top->construct_or_refurbish_phi(&inst, v_table);
    }

    static void init_phi_args(basic_block *block, std::stack<join_block *> &joins, table & v_table){

        if(joins.empty())
            return;

        join_block * top = joins.top();
        top->init_phi_args(v_table);
    }

    //EXACT same traversal as networking switches!!! Freaking cool!!!
    static void phi_versioning(basic_block *block, std::stack<join_block *> &joins){

        if(std::find(visited.begin(), visited.end(), block->node_num) != visited.end())
            return;
        visited.push_back(block->node_num);

        //get most relevant variable version table
        table * new_table = joins.empty() ? &variable_version_table : &joins.top()->v_table;

        if(parents_been_visited(block)){

            //add phi's without base identifiers, to the block
            auto temp = add_phis_to_block(block, joins);
            construct_phis(block, joins, recently_popped == nullptr ? variable_version_table : recently_popped->v_table);
            version_phis_bases(block, recently_popped == nullptr ? temp->v_table : recently_popped->v_table);
            recently_popped = temp;
        } else {
            construct_phis(block, joins, *new_table);
        }

        //add version assignments to all arguments (except phi's)
        version_assignment_args(block, *new_table);

        //take care of phi's here
        init_phi_args(block, joins, *new_table);

        if(block->initial != nullptr && //if i have a child
            (block->initial->mother == nullptr || //and its an only child. else, i must be the mother to recurse
                (block->initial->mother != nullptr && block == block->initial->mother))){

            //push a join block onto the stack if i branch
            if(block->alternate != nullptr)
                joins.push(new join_block(*new_table));

            phi_versioning(block->initial, joins);
        }


        if(block->alternate != nullptr){
            joins.top()->from_initial = false;
            phi_versioning(block->alternate, joins);
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

    static join_block * add_phis_to_block(basic_block *block, std::stack<join_block *> &joins){
        join_block * top = joins.top();
        joins.pop();

        for(auto &phi : top->phi_insts)
            block->instructions.insert(block->instructions.begin(), phi.phi_to_instruction());

        return top;
    }

    static bool parents_been_visited(basic_block *block){
        return block->father != nullptr && block->mother != nullptr;
    }

    static table version_assignment_args(basic_block *block, table & v_table){

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

        return v_table;

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

    static void version_phis_bases(basic_block *block, table &v_table){

        //iterate through block instructions
        for(auto &e : block->instructions)
            if(e.operation == IR_MNEMONIC::PHI){
                auto element = v_table.find(e.arguments[0].var);
                if(element == v_table.end())
                    throw syntax_error();

                e.arguments[0].var.append("_" + std::to_string(++(*element).second));
            }

    }

};

table SSA::variable_version_table;
join_block * SSA::recently_popped;
std::vector<unsigned long> SSA::visited;

#endif //INC_241COMPILER_SSA_HPP
