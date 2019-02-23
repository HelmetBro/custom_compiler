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

typedef unsigned long ulong;
typedef std::unordered_map<ulong, std::vector<ulong>> map;

class IR_builder{

private:

    //used for keeping track of variable versions for phi and branching
    std::unordered_map<std::string, int> version_table;

    block * AST_start = nullptr;
    basic_block * IR_start = nullptr;

    //int types are node numbers
    map dominator_tree;
    std::vector<ulong> visited_nodes; //helps with building dom tree

    bool contains(std::vector<ulong> &list, const ulong &element){
        return std::find(list.begin(), list.end(), element) != list.end();
    }

    bool contains(map &list, ulong element){
        return list.find(element) != list.end();
    }

public:

    void debug(const std::string &num){
        debug::graph(IR_start, dominator_tree, num);
        debug::open(num);
    }

    IR_builder(block * start){
        AST_start = start;

        //storing variables from main
        for(auto v : dynamic_cast<main_block *>(AST_start)->variables) //add vars to version table
            for(auto ident : v->idents) //insert all variable names
                version_table.insert(std::pair<std::string, int>(ident, 0));

        //store function stuff later
        IR_start = new basic_block();
    }

    //recurse over every node
    void build_dominator_tree(){

        dominator_tree.clear();

        //add all nodes to tree, with first dominator being itself
        for(ulong i = 0; i < basic_block::current_node_num; i++){
            dominator_tree.insert(std::pair<ulong, std::vector<ulong>>(i, std::vector<ulong>()));
            dominator_tree[i].push_back(i);
        }

        //construct father dominators
        map points_to = dominator_tree; //deep copy

        visited_nodes.clear();
        create_points_to(IR_start, points_to);

        //O(n^4). yeah. kill me. works for now, refactoring later. i can do better than this
        bool change;
        do{

            change = false;

            //iterate over dominator graph
            for(ulong inode = 0; inode < basic_block::current_node_num; ++inode){

                //elements that point to dominator index
                std::vector<ulong> references = elements_that_point_to(points_to, inode);
                std::vector<ulong> intersection = intersection_of_dominators_of_references(references, inode);

                intersection.push_back(inode);

                if(intersection != dominator_tree[inode]){
                    dominator_tree[inode] = intersection;
                    change = true;
                }
            }

        }while(change);

    }

    std::vector<ulong> intersection(std::vector<ulong> &v1, std::vector<ulong> &v2){
        std::vector<ulong> v3;

        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());

        std::set_intersection(v1.begin(),v1.end(),
                              v2.begin(),v2.end(),
                              back_inserter(v3));
        return v3;
    }

    std::vector<ulong> intersection_of_dominators_of_references(std::vector<ulong> &references, ulong inode){

        if(references.empty())
            return std::vector<ulong>(); //dominator_tree[inode];
        if(references.size() == 1)
            return dominator_tree[references[0]];

        std::vector<ulong> doms1 = dominator_tree[references[0]];
        std::vector<ulong> doms2 = dominator_tree[references[1]];

        return intersection(doms1, doms2);
    }

    std::vector<ulong> elements_that_point_to(const map &points_to, ulong inode){

        std::vector<ulong> elements;

        for(const auto &i : points_to){

            std::vector<ulong> element_set = i.second;

            if(contains(element_set, inode) && i.first < inode){

                elements.push_back(i.first);

            }

        }

        return elements;
    }

    void create_points_to(basic_block *block, map &points_to){

        if(block->initial)
            points_to[block->node_num].push_back(block->initial->node_num);
        if(block->alternate)
            points_to[block->node_num].push_back(block->alternate->node_num);

        //recurse
        visited_nodes.push_back(block->node_num);
        if(block->initial && !contains(visited_nodes, block->initial->node_num))
            create_points_to(block->initial, points_to);
        if(block->alternate && !contains(visited_nodes, block->alternate->node_num))
            create_points_to(block->alternate, points_to);
    }

    void populate_nodes(basic_block *block){

        //all my predecessors dominators are my dominators
        visited_nodes.push_back(block->node_num);
        for(ulong i = 0; i < basic_block::current_node_num; i++){

            if(i == block->node_num)
                continue;

            if(contains(dominator_tree[i], block->node_num))
                dominator_tree[block->node_num].push_back(i);

        }

        //fill map with all reachable nodes per every node
        if(block->initial && !contains(visited_nodes, block->initial->node_num))
            populate_nodes(block->initial);
        if(block->alternate && !contains(visited_nodes, block->alternate->node_num))
            populate_nodes(block->alternate);
    }

    //recurse over every sub-node
    void reachable_nodes(basic_block * block, std::vector<ulong> * list){

        if(block->initial && std::find(list->begin(), list->end(), block->initial->node_num) == list->end()){
            list->push_back(block->initial->node_num);
            reachable_nodes(block->initial, list);
        }

        if(block->alternate && std::find(list->begin(), list->end(), block->alternate->node_num) == list->end()){
            list->push_back(block->alternate->node_num);
            reachable_nodes(block->alternate, list);
        }

    }

    void build_initial_IR(){
        basic_block::current_node_num = 0;
        construct_basic_blocks(IR_start, dynamic_cast<main_block *>(AST_start)->body);
    }

    basic_block * construct_basic_blocks(basic_block * start_basic, body_block * current_body){

        //not taking care of functions for now +_*

        if(current_body != nullptr){
            for(auto s : current_body->statements){
                statement_to_instructions(start_basic->instructions, s);
                start_basic = fill_body(start_basic, s);
            }
        }

        start_basic->print();
        std::cout << std::endl;
        return start_basic;
    }

    basic_block * fill_body(basic_block * current_block, statement * s){

        auto * ending = current_block;

        if(s->type == statement::STATEMENT_TYPE::IF){

            auto * if_stat = dynamic_cast<if_statement *>(s);
            ending = new basic_block();

            /*
             * The "initial" points to the true block (CMP condition fails), vice-versa.
             * */

            //fill true case (CMP condition fails)
            current_block->initial = new basic_block();
//            current_block->initial->initial = ending;
            construct_basic_blocks(current_block->initial, if_stat->true_body)->initial = ending;

            //fill false case (CMP condition succeeds)
            if(if_stat->false_body != nullptr){ //has else block
                current_block->alternate = new basic_block();
//                current_block->alternate->initial = ending;
                construct_basic_blocks(current_block->alternate, if_stat->false_body)->initial = ending;
            } else { //does not have else block
                current_block->alternate = ending;
            }

        }

        if(s->type == statement::STATEMENT_TYPE::WHILE){

            auto * while_stat = dynamic_cast<while_statement *>(s);
            ending = new basic_block();

            /*
             * The "initial" points to the true block (CMP condition fails), vice-versa.
             * */

            //fill true case (CMP condition fails)
            current_block->initial = new basic_block();
            construct_basic_blocks(current_block->initial, while_stat->true_body)->initial = current_block;

            //fill false case (CMP condition succeeds)
            current_block->alternate = ending; //ending block

        }

        return ending;
    }

    void statement_to_instructions(std::vector<instruction> &instructions, statement * s){

        switch(s->type){

                //ASSIGNMENT
            case statement::STATEMENT_TYPE::ASSIGNMENT: {
                auto assign = dynamic_cast<assignment *>(s);
                argument * temp = parse_expression(instructions, assign->exp);
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        IR_MNEMONIC::MOVE, //type of instruction
                        argument(assign->des->ident, argument::ARG_TYPE::VAR), //arg1
                        *temp)); //arg2

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
                instructions.emplace_back(instruction(
                        ++instruction::instruction_counter, //line number
                        IR_MNEMONIC::F_CALL, //type of instruction
                        argument(func->name, argument::ARG_TYPE::FUNC_CALL))); //arg1

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
        else
            arg1 = new argument(tuh->initial_term);
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

};

#endif
