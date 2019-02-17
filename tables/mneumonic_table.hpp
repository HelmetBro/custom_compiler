//
// Created by Eric Parsons on 2019-02-05.
//

#ifndef MNEUMONIC_TABLE_H_
#define MNEUMONIC_TABLE_H_

#include <string>
#include <unordered_map>

#pragma once

enum IR_MNEMONIC{

    //arithmetic
    NEG,
    ADD,
    SUB,
    MUL,
    DIV,
    CMP,

    //memory
    ADDA,
    LOAD,
    STORE,
    MOVE,
    PHI,

    //branching
    END,
    BRA,
    BNE,
    BEQ,
    BLE,
    BLT,
    BGE,
    BGT,

    //functions
    RET,
    F_CALL,

    //other
    READ,
    WRITE,
    WRITENL
};

static std::unordered_map<int, std::string> mneumonic_table ({
                                                                     {IR_MNEMONIC::NEG, "neg"},
                                                                     {IR_MNEMONIC::ADD, "add"},
                                                                     {IR_MNEMONIC::SUB, "sub"},
                                                                     {IR_MNEMONIC::MUL, "mul"},
                                                                     {IR_MNEMONIC::DIV, "div"},
                                                                     {IR_MNEMONIC::CMP, "cmp"},

                                                                     {IR_MNEMONIC::ADDA, "adda"},
                                                                     {IR_MNEMONIC::LOAD, "load"},
                                                                     {IR_MNEMONIC::STORE, "store"},
                                                                     {IR_MNEMONIC::MOVE, "move"},
                                                                     {IR_MNEMONIC::PHI, "phi"},

                                                                     {IR_MNEMONIC::END, "end"},
                                                                     {IR_MNEMONIC::BRA, "bra"},
                                                                     {IR_MNEMONIC::BNE, "bne"},
                                                                     {IR_MNEMONIC::BEQ, "beq"},
                                                                     {IR_MNEMONIC::BLE, "ble"},
                                                                     {IR_MNEMONIC::BLT, "blt"},
                                                                     {IR_MNEMONIC::BGE, "bge"},
                                                                     {IR_MNEMONIC::BGT, "bgt"},

                                                                     {IR_MNEMONIC::RET, "ret"},
                                                                     {IR_MNEMONIC::F_CALL, "call"},

                                                                     {IR_MNEMONIC::READ, "read"},
                                                                     {IR_MNEMONIC::WRITE, "write"},
                                                                     {IR_MNEMONIC::WRITENL, "writeNL"}
});

#endif
