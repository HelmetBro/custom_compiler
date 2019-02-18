//
// Created by Eric Parsons on 2019-02-02.
//

#ifndef IR_BUILDER_H_
#define IR_BUILDER_H_

#include "blocks/main_block.hpp"
#include "blocks/statements/assignment.hpp"
#include "blocks/statements/return_statement.hpp"
#include "blocks/statements/if_statement.hpp"
#include "blocks/statements/while_statement.hpp"

#include "IR_parts/basic_block.hpp"
#include "IR_parts/debug.hpp"

class IR_builder{

private:

    //used for keeping track of variable versions for phi and branching
    std::unordered_map<std::string, int> version_table;

    block * AST_start = nullptr;
    basic_block * IR_start = nullptr;

public:

    void debug(){
        debug::graph(IR_start);
    }

    IR_builder(block * start){
        AST_start = start;

        //storing variables from main
        for(auto v : dynamic_cast<main_block *>(AST_start)->variables) //add vars to version table
            for(auto ident : v->idents) //insert all variable names
                version_table.insert(std::pair<std::string, int>(ident, 0));

        //store function stuff later

        IR_start = new basic_block();
        body_block * current_block = dynamic_cast<main_block *>(AST_start)->body;
        construct_basic_blocks(IR_start, current_block);
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
            current_block->initial->initial = ending;
            construct_basic_blocks(current_block->initial, if_stat->true_body)->initial = ending;

            //fill false case (CMP condition succeeds)
            if(if_stat->false_body != nullptr){ //has else block
                current_block->alternate = new basic_block();
                current_block->alternate->initial = ending;
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
