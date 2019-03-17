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

    explicit join_block(const table *initial, const table *alternate){

        for(auto &i : *initial){

            //if initial's version is not alternates, make a phi
            if(i.second != alternate->at(i.first)){

                //initial's version
                std::string phi_arg_1(i.first);
                phi_arg_1.append("_" + std::to_string(i.second));

                std::string phi_arg_2(i.first);
                phi_arg_2.append("_" + std::to_string(alternate->at(i.first)));

                phi_insts.emplace_back(phi_instruction(++instruction::instruction_counter, i.first, phi_arg_1, phi_arg_2));
            }
        }

    }

};

class SSA{

public:

    //every ssa pass update this variable
    static void ssa(basic_block * IR_start, std::unordered_map<std::string, int> version_table){
        variable_version_table = std::move(version_table);

        visited.clear();
        auto join_stack = std::stack<join_block *>();
        phi_versioning(IR_start, variable_version_table);
    }

private:

    static std::vector<unsigned long> visited;
    static table variable_version_table;

    static table * phi_versioning(basic_block *block, table v_table){

        //only time this is true, is during while loops
        bool is_while_block = false;
        if(std::find(visited.begin(), visited.end(), block->node_num) != visited.end())
            return nullptr;
        visited.push_back(block->node_num);

        //making copy, just in case it's a while loop block
        auto while_table = new table(v_table);

        //version phis, update globals
        version_phis_bases(block, &v_table);
        variable_version_table = *combine_tables(&variable_version_table, &v_table);

        //version assignment arguments, update globals
        version_assignment_args(block, &v_table);
        variable_version_table = *combine_tables(&variable_version_table, &v_table);

        auto initial = new table(v_table);
        auto alternate = new table(v_table);

        if(block->initial != nullptr && block->initial->mother == nullptr){
            delete initial;
            initial = phi_versioning(block->initial, v_table);
        }

        //if initial is nullptr, then we hit a while loop block
        if(initial == nullptr){
            auto join = join_block(&v_table, while_table);
            add_phis_to_block(block->initial, join);
            version_phis_bases(block->initial, &v_table);
            variable_version_table = *combine_tables(&variable_version_table, &v_table);
        }

        //check if we're the head of a while loop, combine v_table with new versions and carry on

        if(block->alternate != nullptr && block->alternate != block->ending){
            delete alternate;
            alternate = phi_versioning(block->alternate, v_table);
        }

        //next block in sequence
        if(block->ending != nullptr){

            auto join = join_block(initial, alternate);

            //add join block to beginning of ending block, and continue
            if(!block->is_while)
                add_phis_to_block(block->ending, join);

            table combined = *combine_tables(initial, alternate);

            delete initial;
            delete alternate;
            return phi_versioning(block->ending, combined);
        }

        delete initial;
        delete alternate;
        return new table(v_table);

//        //EXACT same traversal as networking switches!!! Freaking cool!!!
//        if(std::find(visited.begin(), visited.end(), block->node_num) != visited.end())
//            return;
//        visited.push_back(block->node_num);
//
//        //get most relevant variable version table
//        table * new_table = joins.empty() ? &variable_version_table : &joins.top()->v_table;
//
//        if(parents_been_visited(block)){
//
//            //add phi's without base identifiers, to the block
//            auto temp = add_phis_to_block(block, joins);
//            construct_phis(block, joins, recently_popped == nullptr ? variable_version_table : recently_popped->v_table);
//            version_phis_bases(block, recently_popped == nullptr ? temp->v_table : recently_popped->v_table);
//            recently_popped = temp;
//        } else {
//            construct_phis(block, joins, *new_table);
//        }
//
//        //add version assignments to all arguments (except phi's)
//        version_assignment_args(block, *new_table);
//
//        //take care of phi's here
//        init_phi_args(block, joins, *new_table);
//
//        if(block->initial != nullptr && //if i have a child
//            (block->initial->mother == nullptr || //and its an only child. else, i must be the mother to recurse
//                (block->initial->mother != nullptr && block == block->initial->mother))){
//
//            //push a join block onto the stack if i branch
//            if(block->alternate != nullptr)
//                joins.push(new join_block(*new_table));
//
//            phi_versioning(block->initial, joins);
//        }
//
//
//        if(block->alternate != nullptr){
//            joins.top()->from_initial = false;
//            phi_versioning(block->alternate, joins);
//        }

    }

    static table * combine_tables(const table * first, const table * second){

        auto * temp = new table(*first);

        for(auto &e : *temp)
            if(e.second < second->at(e.first))
                e.second = second->at(e.first);

        return temp;
    }

    static void add_phis_to_block(basic_block *block, join_block & join){
        for(auto &phi : join.phi_insts)
            block->instructions.insert(block->instructions.begin(), phi.phi_to_instruction());
    }

    static void version_assignment_args(basic_block *block, table * v_table){

        //iterate through block instructions
        for(auto &e : block->instructions)

                //iterate through all of its arguments
                for(int i = static_cast<int>(e.arguments.size() - 1); i >= 0; i--){

                    //if we're a phi operation, then skip it
                    if(e.operation == IR_MNEMONIC::PHI)
                        continue;

                    //if we're a variable
                    if(e.arguments[i].type == argument::ARG_TYPE::VAR)

                        //then version it!
                        version_arg(e.arguments[i].var, e.operation, i == 0, v_table);
                }
    }

    static void version_arg(std::string &var, IR_MNEMONIC op, bool first_arg, table * v_table){

        table * v_t = v_table;

        if(first_arg && op == IR_MNEMONIC::MOVE)
            v_t = &variable_version_table;

        auto element = v_t->find(var);
        if(element == v_t->end())
            throw syntax_error();

        if(first_arg && op == IR_MNEMONIC::MOVE){
            ++(*element).second;
            v_table->at(element->first) = element->second;
        }

        var.append("_" + std::to_string((*element).second));
    }

    static void version_phis_bases(basic_block *block, table * v_table){

        //iterate through block instructions
        for(auto &e : block->instructions)
            if(e.operation == IR_MNEMONIC::PHI){
                auto element = v_table->find(e.arguments[0].var);
                if(element == v_table->end())
                    throw syntax_error();

                e.arguments[0].var.append("_" + std::to_string(++(*element).second));
            }

    }

};

table SSA::variable_version_table;
std::vector<unsigned long> SSA::visited;

#endif //INC_241COMPILER_SSA_HPP
