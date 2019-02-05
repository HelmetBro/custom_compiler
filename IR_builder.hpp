//
// Created by Eric Parsons on 2019-02-02.
//

#ifndef IR_BUILDER_H_
#define IR_BUILDER_H_

#include "blocks/block.hpp"
#include "instruction.hpp"
#include "blocks/main_block.hpp"
#include "blocks/statements/assignment.hpp"

unsigned long instruction_counter = 0;

class basic_Block{

    std::vector<instruction> instructions;

    //optional
    basic_Block * initial = nullptr; //the "true" case
    basic_Block * alternate = nullptr; //the "false" case

public:

    void print(){
        for(auto i : instructions)
            i.print();
    }

    basic_Block(body_block * body){

        //create instructions from statements
        for(auto s : body->statements){

            switch(s->type){

                case statement::STATEMENT_TYPE::ASSIGNMENT:
                    auto assign = dynamic_cast<assignment *>(s);

                    expression_to_instructions(assign->exp);
                    print();

//                    instructions.insert(instruction(
//                            instruction_counter++,
//                            instruction::IR_MNEMONIC::MOVE,
//                            //get instruction number or var or const
//                            ,assign->des->ident));
                    break;


            }



        }

    }


    //argument returned from here will always be an instruction line
    argument * expression_to_instructions(expression * exp){

        argument * arg1 = nullptr;
        argument * arg2 = nullptr;

        if(exp->initial_term != nullptr)
            arg1 = term_to_instructions(exp->initial_term);
        if(exp->optional_term != nullptr)
            arg2 = expression_to_instructions(exp->optional_term);

        if(exp->plus_minus == ADD)
            instructions.emplace_back(++instruction_counter, instruction::IR_MNEMONIC::ADD, *arg1, *arg2);
        else if(exp->plus_minus == SUB)
            instructions.emplace_back(++instruction_counter, instruction::IR_MNEMONIC::SUB, *arg1, *arg2);

        if(arg1 != nullptr && arg2 != nullptr)
            return new argument((int)instruction_counter, argument::ARG_TYPE::INSTRUCT);
        else
            return arg1;
    };

    argument * term_to_instructions(term * tuh){

        argument * arg1 = nullptr;
        argument * arg2 = nullptr;

        if(tuh->initial_term->exp != nullptr)
            arg1 = expression_to_instructions(tuh->initial_term->exp);
        if(tuh->optional_term != nullptr)
            arg2 = term_to_instructions(tuh->optional_term);

        if(tuh->times_divide == MUL)
            instructions.emplace_back(++instruction_counter, instruction::IR_MNEMONIC::MUL, *arg1, *arg2);
        else if(tuh->times_divide == DIV)
            instructions.emplace_back(++instruction_counter, instruction::IR_MNEMONIC::DIV, *arg1, *arg2);

        return new argument(tuh->initial_term);
    }

};

class IR_builder{

private:

    //used for keeping track of variable versions for phi and branching
    std::unordered_map<std::string, int> version_table;

    block * AST_start = nullptr;
    basic_Block * IR_start = nullptr;

public:

    IR_builder(block * start){
        AST_start = start;
    }

    void construct_basic_blocks(){

        block * current_AST = AST_start;

        switch (current_AST->type){

            case block::BLOCK_TYPE::MAIN:
                auto * main = dynamic_cast<main_block *>(current_AST);

                for(auto v : main->variables) //add vars to version table
                    for(auto ident : v->idents) //insert all variable names
                        version_table.insert(std::pair<std::string, int>(ident, 0));

                //not taking care of functions for now +_*


                IR_start = new basic_Block(main->body);

                break;


        }


    }

};

#endif
