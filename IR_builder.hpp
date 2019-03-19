//
// Created by Eric Parsons on 2019-02-02.
//

#ifndef IR_BUILDER_H_
#define IR_BUILDER_H_

#include <algorithm>

#include "blocks/main_block.hpp"
#include "blocks/statements/assignment.hpp"
#include "blocks/statements/return_statement.hpp"
#include "blocks/statements/if_statement.hpp"
#include "blocks/statements/while_statement.hpp"

#include "IR_parts/basic_block.hpp"
#include "IR_parts/debug.hpp"
#include "IR_parts/passes/SSA.hpp"

typedef std::unordered_map<basic_block*, std::vector<basic_block*>> map;

class IR_builder{

private:

    //used for keeping track of variable versions for phi and branching
    std::unordered_map<std::string, int> version_table;

    block * AST_start = nullptr;
    basic_block * IR_start = nullptr;
    std::vector<basic_block *> functions;

    //int types are node numbers
    map dominator_tree;

    bool contains(const map &list, basic_block* block){
        return list.find(block) != list.end();
    }

public:

    basic_block * get_main(){
        return IR_start;
    }

    std::vector<basic_block *> get_functions(){
        return functions;
    }

    table get_variables(){
        return version_table;
    }

    void debug(const std::string &num, bool doms, bool parents){
        debug::graph(IR_start, functions, dominator_tree, num, doms, parents);
        debug::open(num);
    }

    explicit IR_builder(block * start){
        AST_start = start;

        //mains' variables
        for(auto v : dynamic_cast<main_block *>(AST_start)->variables)
            for(auto ident : v->idents)
                version_table.insert(std::pair<std::string, int>(ident, 0));

        //functions' variables
        for(auto &func : dynamic_cast<main_block *>(AST_start)->functions)
            for(auto ident : func->variables)
                for(auto v : ident->idents)
                    version_table.insert(std::pair<std::string, int>(v, 0));

        //resets
        instruction::instruction_counter = 0;
        basic_block::current_node_num = 0;

        //main
        IR_start = new basic_block();
    }

    //recurse over every node
    void build_dominator_tree(){

        dominator_tree.clear();
        populate_map_with_nodes(IR_start, dominator_tree);

        bool change;
        do{

            change = false;

            for(const auto &key : dominator_tree){
                std::vector<basic_block *> parents = get_parents(key.first);
                std::vector<basic_block *> intersection = intersection_of_parent_doms(parents);

                intersection.emplace_back(key.first);

                if(intersection != key.second){
                    dominator_tree[key.first] = intersection;
                    change = true;
                }

            }

        }while(change);

    }

    void populate_map_with_nodes(basic_block* block, std::unordered_map<basic_block*, std::vector<basic_block*>> &dominator_tree){

        dominator_tree.insert({block, std::vector<basic_block*>()});

        if(block->initial && !contains(dominator_tree, block->initial))
            populate_map_with_nodes(block->initial, dominator_tree);
        if(block->alternate && !contains(dominator_tree, block->alternate))
            populate_map_with_nodes(block->alternate, dominator_tree);
    }

    std::vector<basic_block*> intersection(std::vector<basic_block*> &v1, std::vector<basic_block*> &v2){
        std::vector<basic_block*> v3;

        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());

        std::set_intersection(v1.begin(),v1.end(),
                              v2.begin(),v2.end(),
                              back_inserter(v3));
        return v3;
    }

    std::vector<basic_block *> intersection_of_parent_doms(std::vector<basic_block *> &parents){

        if(parents.empty())
            return std::vector<basic_block *>();

        if(parents.size() == 1)
            return parents;

        std::vector<basic_block*> doms1 = dominator_tree[parents[0]];
        std::vector<basic_block*> doms2 = dominator_tree[parents[1]];

        return intersection(doms1, doms2);
    }

    std::vector<basic_block *> get_parents(const basic_block* block){

        std::vector<basic_block *> elements;

        if(block->father)
            elements.push_back(block->father);
        if(block->mother)
            elements.push_back(block->mother);

        return elements;
    }

    void build_initial_IR(){
        construct_basic_blocks(IR_start, dynamic_cast<main_block *>(AST_start)->body);

        for(auto &func : dynamic_cast<main_block *>(AST_start)->functions){
            auto func_block = new basic_block();
            construct_basic_blocks(func_block, func->body);
            functions.emplace_back(func_block);
        }

    }

    basic_block * construct_basic_blocks(basic_block * start_basic, body_block * current_body){

        //not taking care of functions for now +_*

        if(current_body != nullptr){
            for(auto s : current_body->statements){
                statement_to_instructions(start_basic->instructions, s);
                start_basic = fill_body(start_basic, s);
            }
        }

        return start_basic;
    }

    basic_block * fill_body(basic_block * current_block, statement * s){

        auto * ending = current_block;

        if(s->type == statement::STATEMENT_TYPE::IF){
            auto * if_stat = dynamic_cast<if_statement *>(s);
            current_block->is_if = true;
            current_block->ending = ending = new basic_block(); //node num not correctly updating

            /*
             * The "initial" points to the true block (CMP condition fails), vice-versa.
             * */

            //fill true case (CMP condition fails)
            current_block->initial = new basic_block();
            current_block->initial->father = current_block;
            auto deepest_init = construct_basic_blocks(current_block->initial, if_stat->true_body);
            deepest_init->initial = ending;
            ending->father = deepest_init;

            //fill false case (CMP condition succeeds)
            if(if_stat->false_body != nullptr){ //has else block
                current_block->alternate = new basic_block();
                current_block->alternate->father = current_block;
                auto deepest_alt = construct_basic_blocks(current_block->alternate, if_stat->false_body);
                deepest_alt->initial = ending;
                ending->mother = deepest_alt;

            } else { //does not have else block
                current_block->alternate = ending;
                ending->mother = current_block;
            }
        }

        if(s->type == statement::STATEMENT_TYPE::WHILE){
            auto * while_stat = dynamic_cast<while_statement *>(s);
            current_block->is_while = true;
            current_block->ending = ending = new basic_block();

            /*
             * The "initial" points to the true block (CMP condition fails), vice-versa.
             * */

            //fill true case (CMP condition fails)
            current_block->initial = new basic_block();
            current_block->initial->father = current_block;
            auto deepest = construct_basic_blocks(current_block->initial, while_stat->true_body);
            deepest->initial = current_block;

            //fill false case (CMP condition succeeds)
            current_block->alternate = ending; //ending block
            ending->father = current_block;

        }

        return ending;
    }

    void statement_to_instructions(std::vector<instruction> &instructions, statement * s){

        switch(s->type){

                //ASSIGNMENT
            case statement::STATEMENT_TYPE::ASSIGNMENT: {
                auto assign = dynamic_cast<assignment *>(s);
                argument * first = assign->des->is_array ? make_array(instructions, assign->des) : new argument(assign->des->ident, argument::ARG_TYPE::VAR);
                argument * second = parse_expression(instructions, assign->exp);
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        IR_MNEMONIC::MOVE, //type of instruction
                        *first, //arg1
                        *second)); //arg2

                break;
            }

                //RETURN
            case statement::STATEMENT_TYPE::RETURN: {
                auto ret = dynamic_cast<return_statement *>(s);
                argument * temp = parse_expression(instructions, ret->exp);
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        IR_MNEMONIC::RET, //type of instruction
                        *temp)); //arg1

                break;
            }

                //FUNCTION CALL
            case statement::STATEMENT_TYPE::FUNC_CALL:{
                auto func = dynamic_cast<function_call *>(s);

                /* We have "special" functions, InputNum, OutputNum, OutputNewLine.
                 * Only the function OutputNum has arguments, and will have a special call.*/

                if(func->name == "OutputNum"){
                    instructions.emplace_back(instruction(
                            ++instruction::instruction_counter,
                            IR_MNEMONIC::WRITE,
                            parse_expression(instructions, func->arguments.front())));

                } else if (func->name == "OutputNewLine"){
                    instructions.emplace_back(instruction(
                            ++instruction::instruction_counter,
                            IR_MNEMONIC::WRITENL));

                } else if (func->name == "InputNum"){
                    instructions.emplace_back(instruction(
                            ++instruction::instruction_counter,
                            IR_MNEMONIC::READ));

                } else {
                    instructions.emplace_back(instruction(
                            ++instruction::instruction_counter,
                            IR_MNEMONIC::F_CALL,
                            argument(func->name, argument::ARG_TYPE::FUNC_CALL)));
                }

                break;
            }

                //WHILE STATEMENTS
            case statement::STATEMENT_TYPE::WHILE:{
                auto while_stat = dynamic_cast<while_statement *>(s);

                //compare instruction
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        IR_MNEMONIC::CMP,
                        argument(*parse_expression(instructions, while_stat->condition->left_expression)), //arg1
                        argument(*parse_expression(instructions, while_stat->condition->right_expression)) //arg2
                ));

                //branching instruction
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        while_stat->condition->inverse_condition() //inverse condition instruction
                ));

                break;
            }

                //IF STATEMENTS
            case statement::STATEMENT_TYPE::IF:{
                auto if_stat = dynamic_cast<if_statement *>(s);

                //compare instruction
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        IR_MNEMONIC::CMP,
                        argument(*parse_expression(instructions, if_stat->condition->left_expression)), //arg1
                        argument(*parse_expression(instructions, if_stat->condition->right_expression)) //arg2
                ));

                //branching instruction
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        if_stat->condition->inverse_condition() //inverse condition instruction
                ));

                break;
            }

            case statement::STATEMENT_TYPE::END:{
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        IR_MNEMONIC::END)); //arg2
                break;
            }

        }//switch

    }

    //argument returned from here will always be an instruction line
    /* TODO: LATER, MAKE IF's INTO CONDITIONALS */
    argument * parse_expression(std::vector<instruction> &instructions, expression *exp){

        argument * arg1 = nullptr;
        argument * arg2 = nullptr;

        if(exp->initial_term != nullptr)
            arg1 = parse_term(instructions, exp->initial_term); //
        if(exp->optional_term != nullptr)
            arg2 = parse_expression(instructions, exp->optional_term);

        if(exp->plus_minus == SYMBOL::ADDITION)
            instructions.emplace_back(++instruction::instruction_counter, IR_MNEMONIC::ADD, *arg1, *arg2);
        else if(exp->plus_minus == SYMBOL::SUBTRACTION)
            instructions.emplace_back(++instruction::instruction_counter, IR_MNEMONIC::SUB, *arg1, *arg2);

        if(arg1 != nullptr && arg2 != nullptr)
            return new argument((int)instruction::instruction_counter, argument::ARG_TYPE::INSTRUCT);

        return arg1;
    };
    argument * parse_term(std::vector<instruction> &instructions, term *tuh){

        argument * arg1 = nullptr;
        argument * arg2 = nullptr;

        if(tuh->initial_term->exp != nullptr)
            arg1 = parse_expression(instructions, tuh->initial_term->exp);
        else if (tuh->initial_term->des != nullptr && !tuh->initial_term->des->exp.empty())
            return make_array(instructions, tuh->initial_term->des);
        else
            arg1 = tuh->initial_term->des != nullptr && is_array(tuh->initial_term->des->ident) ?
                    new argument(tuh->initial_term->des->ident, argument::ADDR):
                    new argument(tuh->initial_term);
        if(tuh->optional_term != nullptr)
            arg2 = parse_term(instructions, tuh->optional_term);

        if(tuh->times_divide == SYMBOL::MULTIPLICATION)
            instructions.emplace_back(++instruction::instruction_counter, IR_MNEMONIC::MUL, *arg1, *arg2);
        else if(tuh->times_divide == SYMBOL::DIVISION)
            instructions.emplace_back(++instruction::instruction_counter, IR_MNEMONIC::DIV, *arg1, *arg2);

        if(arg1 != nullptr && arg2 != nullptr)
            return new argument((int)instruction::instruction_counter, argument::ARG_TYPE::INSTRUCT);

        if(arg1 != nullptr)
            return arg1;

        return new argument(tuh->initial_term);
    }
    argument * make_array(std::vector<instruction> &instructions, designator * local_des){

        //compute expression of first in array
        argument * result = parse_expression(instructions, local_des->exp.front());

        //specifier
        int specifier_inst = static_cast<int>(++instruction::instruction_counter);
        instructions.emplace_back(instruction::instruction_counter,
                                  IR_MNEMONIC::MUL,
                                  *result,
                                  new argument(4, argument::ARG_TYPE::CONST));

        //offsets
        for(int i = 1; i < local_des->exp.size(); i++){
            int dim_size = get_dimension_size(local_des, i);
            instructions.emplace_back(++instruction::instruction_counter,
                                      IR_MNEMONIC::MUL,
                                      new argument(dim_size, argument::ARG_TYPE::CONST), //size of dimension
                                      new argument(4, argument::ARG_TYPE::CONST)); //int size

            argument * temp = parse_expression(instructions, local_des->exp.at(i));
            instructions.emplace_back(instruction::instruction_counter + 1,
                                      IR_MNEMONIC::MUL,
                                      new argument((int)instruction::instruction_counter, argument::ARG_TYPE::INSTRUCT),
                                      *temp); //int size
            instruction::instruction_counter++;
        }

        //adding specifier and offsets (if applicable)
        if(local_des->exp.size() > 1){
            instructions.emplace_back(instruction::instruction_counter + 1,
                                      IR_MNEMONIC::ADD,
                                      new argument((int)instruction::instruction_counter, argument::ARG_TYPE::INSTRUCT),
                                      new argument(specifier_inst, argument::ARG_TYPE::INSTRUCT)); //int size
            instruction::instruction_counter++;
        }

        //the ADD instruction
        instructions.emplace_back(instruction::instruction_counter + 1,
                                  IR_MNEMONIC::ADDA,
                                  new argument(local_des->ident, argument::ARG_TYPE::ADDR),
                                  new argument((int)instruction::instruction_counter, argument::ARG_TYPE::INSTRUCT));
        instruction::instruction_counter++;

        return new argument((int)instruction::instruction_counter, argument::ARG_TYPE::INSTRUCT);
    }

    bool is_array(const std::string &var){

        bool _is = false;

        //terrible programming, i know. I'm on a deadline though.

        for(auto v : dynamic_cast<main_block *>(AST_start)->variables)
            for(const auto &ident : v->idents)
                if(ident == var)
                    return v->is_array;

        //function variables
        for(auto &func : dynamic_cast<main_block *>(AST_start)->functions)
            for(auto ident : func->variables)
                for(const auto &v : ident->idents)
                    if(v == var)
                        return ident->is_array;

        return _is;
    }

    int get_dimension_size(designator *des, int dim_index){
        //getting dimension size
        auto main_block = dynamic_cast<class main_block *>(AST_start);

        for(auto &e : main_block->variables){
            auto element = std::find(e->idents.begin(), e->idents.end(), des->ident);
            if(element != e->idents.end())
                return e->numbers.at(dim_index);
        }
    }

    void ssa(){
        SSA::ssa_add_phis(IR_start, functions, version_table);
        SSA::ssa_remove_phis(IR_start, functions, version_table);
    }

};

#endif
