//
// Created by eric on 3/17/19.
//

#ifndef INC_241COMPILER_DLX_BUILDER_H
#define INC_241COMPILER_DLX_BUILDER_H

#include <string>
#include <numeric>

#include "IR_parts/basic_block.hpp"

class DLX_builder{

private:

    const int ADD = 0;
    const int SUB = 1;
    const int MUL = 2;
    const int DIV = 3;
    const int MOD = 4;
    const int CMP = 5;
    const int OR  = 8;
    const int AND = 9;
    const int BIC = 10;
    const int XOR = 11;
    const int LSH = 12;
    const int ASH = 13;
    const int CHK = 14;

    const int ADDI = 16;
    const int SUBI = 17;
    const int MULI = 18;
    const int DIVI = 19;
    const int MODI = 20;
    const int CMPI = 21;
    const int ORI  = 24;
    const int ANDI = 25;
    const int BICI = 26;
    const int XORI = 27;
    const int LSHI = 28;
    const int ASHI = 29;
    const int CHKI = 30;

    const int LDW = 32;
    const int LDX = 33;
    const int POP = 34;
    const int STW = 36;
    const int STX = 37;
    const int PSH = 38;

    const int BEQ = 40;
    const int BNE = 41;
    const int BLT = 42;
    const int BGE = 43;
    const int BLE = 44;
    const int BGT = 45;
    const int BSR = 46;
    const int JSR = 48;
    const int RET = 49;

    const int RDI = 50; //This is a bug, project template says RDD!
    const int WRD = 51;
    const int WRH = 52;
    const int WRL = 53;

    const int ERR = 63;

    static const int MemSize = 10000; //taken from DLX java file VM
    int M[MemSize][1 + 4];//MemSize x 4 means 4 arguments, VM later smashes into 1. +1 is for instruction identifier

    int _instr = 0; //next instruction number

    int _start_open_regs = 1; //next available register, after globals
    std::string * regs[32];

    //for traversal
    std::vector<unsigned long> visited;

    int RA = 31;
    int GL = 30;
    int SP = 29;
    int FP = 28;


    int prev_FA = 0;
    int FA = prev_FA; //function address
    std::vector<std::pair<std::string, int>> FA_map;

public:

    void build(basic_block * main, const std::vector<basic_block *> &functions, const table &variables, block * AST_main){

        for(int i = 1; i < 28; ++i)
            regs[i] = nullptr;

        //assign registers to global variables
        for(const auto &variable : variables)
            _start_open_regs = allocate_reg(variable.first);
        _start_open_regs++;

        //for all arrays, initialize their registers with locations on heap.
        initialize_arrays(AST_main);

        //put all function names (with FA = 0, and with main) inside of FA_map
        for (auto function : functions)
            FA_map.emplace_back(std::pair<std::string, int>(function->name, 0));
        FA_map.emplace_back(std::pair<std::string, int>(main->name, FA));

        //traversal
        traverse(main);

        for (auto function : functions) {
            update_FA(function->name);
            traverse(function);
        }

        //finally putting functions together
        link_functions();
    }

    void update_FA(const std::string &name){
        prev_FA = FA;
        FA = _instr - 1;
        for (auto &ind : FA_map)
            if(ind.first == name)
                ind.second = FA;
    }

    void link_functions(){

        //find JSR instruction and LINK!
        int index = 0;
        while(index++ < _instr)
            if(M[index][1] == JSR)
                M[index][2] = FA_map.at(static_cast<unsigned long>(M[index][2])).second;

    }

    void initialize_arrays(block * AST_main){

        auto main = dynamic_cast<main_block *>(AST_main);
        int free_space = MemSize;

        //mains' variables
        for(const auto &v : main->variables)
            for(const auto &ident : v->idents)
                if(v->is_array)
                    allocate_array(ident, v->numbers, free_space);

        //functions' variables
        for(const auto &func : main->functions)
            for(const auto &v : func->variables)
                for(const auto &ident : v->idents)
                    if(v->is_array)
                        allocate_array(ident, v->numbers, free_space);

        code(-1, ADDI, SP, 0, free_space - 1);
    }

    void allocate_array(const std::string &var, const std::vector<int> & dimensions, int &free_space){

        int reg = get_reg(var);
        int size = std::accumulate(dimensions.begin(), dimensions.end(), 0);
        int address = free_space -= size;

        //put address into array register
        code(-1, ADDI, reg, 0, address);
    }

    bool traverse(basic_block * block){

        if(std::find(visited.begin(), visited.end(), block->node_num) != visited.end())
            return true;
        visited.push_back(block->node_num);

        int before_inst = _instr;
        bool branches = to_instructions(block);
        int before_trav = _instr - 1;

        //traverse left
        int NOP_location = 0;
        if(block->initial != nullptr && block->initial->mother == nullptr){
            if(traverse(block->initial)) //if we hit a while loop, add branch instruction
                code(-1, BEQ, 0, before_inst -_instr - 2);
            NOP_location = NOP();
        }

        //if we branch, update the branch instruction
        if(branches)
            M[before_trav][3] = _instr - before_trav;

        //traverse right
        if(block->alternate != nullptr && block->alternate != block->ending){
            before_trav = _instr;
            traverse(block->alternate);
            M[NOP_location][1] = BEQ;
            M[NOP_location][3] = _instr - before_trav + 1;
        }

        //finish up
        if(block->ending != nullptr)
            traverse(block->ending);

        return false;
    }

    bool to_instructions(basic_block * block){

        for(int count = 0; count < block->instructions.size(); count++){

            auto &i = block->instructions[count];

            int size = static_cast<int>(i.arguments.size());
            argument * arg1 = (size >= 1 ? &i.arguments.at(0) : nullptr);
            argument * arg2 = (size >= 2 ? &i.arguments.at(1) : nullptr);
            argument * arg3 = (size >= 3 ? &i.arguments.at(2) : nullptr);

            switch (i.operation){

                case NEG: break;
                case ::ADD:
                    add(i, arg1, arg2); break;
                case ::SUB:
                    sub(i, arg1, arg2); break;
                case ::MUL:
                    mul(i, arg1, arg2); break;
                case ::DIV:
                    div(i, arg1, arg2); break;
                case ::CMP:
                    cmp(i, arg1, arg2); break;

                case ADDA:
                    adda(i, arg1, arg2); break;

                case LOAD:break;
                case STORE:break;

                case MOVE:
                    mov(count - 1 > 0 ? &block->instructions[count - 1] : nullptr, i, arg1, arg2); break;
                case PHI:break;
                case END:
                    code(static_cast<int>(i.line_num), RET, 0); break;

                case BRA:
                    code(static_cast<int>(i.line_num), BEQ, 0, 0); return true;
                case ::BNE:
                    code(static_cast<int>(i.line_num), BNE, get_reg(static_cast<int>(i.line_num) - 1), 0); return true;
                case ::BEQ:
                    code(static_cast<int>(i.line_num), BEQ, get_reg(static_cast<int>(i.line_num) - 1), 0); return true;
                case ::BLE:
                    code(static_cast<int>(i.line_num), BLE, get_reg(static_cast<int>(i.line_num) - 1), 0); return true;
                case ::BLT:
                    code(static_cast<int>(i.line_num), BLT, get_reg(static_cast<int>(i.line_num) - 1), 0); return true;
                case ::BGE:
                    code(static_cast<int>(i.line_num), BGE, get_reg(static_cast<int>(i.line_num) - 1), 0); return true;
                case ::BGT:
                    code(static_cast<int>(i.line_num), BGT, get_reg(static_cast<int>(i.line_num) - 1), 0); return true;

                case ::RET:break;

                case F_CALL:
                    call(i, arg1); break;

                case READ:
                    code(static_cast<int>(i.line_num), RDI, allocate_reg()); break;
                case WRITE:
                    code(static_cast<int>(i.line_num), WRD, assign_reg(i, arg1)); break;
                case WRITENL:
                    code(static_cast<int>(i.line_num), WRL); break;
            }

        }

        //clear registers
        clear_variable_registers();

        return false;

    }

    void call(instruction &i, argument * arg1){

        //putting RA in register and stack
        code(-1, ADDI, RA, RA, _instr);
        code(-1, PSH, RA, SP, -4);

        //push parameters on stack!
        for(int p = 1; p < i.arguments.size(); ++p)
            code(static_cast<int>(i.line_num), PSH, get_reg(i.arguments[p].var), SP, -4);

        //gets index of function from function table
        int f_index = -1;
        for(int ind = 0; ind < FA_map.size(); ++ind)
            if(FA_map[ind].first == arg1->var)
                f_index = ind;

        code(-1, JSR, f_index);

        //restore parameters
        for(int p = 1; p < i.arguments.size(); ++p)
            code(static_cast<int>(i.line_num), POP, get_reg(i.arguments[p].var), SP, 4);

        code(-1, POP, RA, SP, 4);

//        //before
//        code(-1, PSH, RA, SP, -4);
//        code(-1, PSH, FP, SP, -4);
//        code(-1, ADD, FP, 0, SP);
//        code(-1, SUBI, SP, SP, static_cast<int>(i.arguments.size() * 4));

        //after
//        code(-1, ADD, SP, 0, FP);
//        code(-1, POP, FP, SP, 4);
//        code(-1, POP, RA, SP, 4);
//        code(-1, RET, 0, 0, RA);

    }

    void arithmetic(int op, int opi, instruction &i, argument * arg1, argument * arg2){
        if((arg1->type == argument::VAR || arg1->type == argument::INSTRUCT) &&
           (arg2->type == argument::VAR || arg2->type == argument::INSTRUCT)){
            code(static_cast<int>(i.line_num),
                 op,
                 allocate_reg(std::to_string((int)i.line_num)),
                 get_reg(arg1->type == argument::VAR ? arg1->var : std::to_string(arg1->value)),
                 get_reg(arg2->type == argument::VAR ? arg2->var : std::to_string(arg2->value)));
        } else if (arg1->type == argument::CONST && arg2->type == argument::CONST) {

            int temp_reg = get_reg(std::to_string(arg1->value));
            if(temp_reg == 0)
                temp_reg = allocate_reg_const(i, arg1->value);

            code(static_cast<int>(i.line_num), opi, allocate_reg(std::to_string((int)i.line_num)), temp_reg, arg2->value);
        } else {

            int reg_result = allocate_reg(std::to_string(static_cast<int>(i.line_num)));
            code(static_cast<int>(i.line_num),
                 opi,
                 reg_result,
                 arg1->type == argument::CONST ?
                 (get_reg(arg2->type == argument::VAR ? arg2->var : std::to_string(arg2->value))):
                 (get_reg(arg1->type == argument::VAR ? arg1->var : std::to_string(arg1->value))),
                 arg1->type == argument::CONST ? arg1->value : arg2->value
            );

            //special case where subtracting order matters
            if(arg1->type == argument::CONST && opi == SUBI)
                code(static_cast<int>(i.line_num), MULI, reg_result, reg_result, -1);

        }


    }

    void mov(instruction * prev, instruction &i, argument * arg1, argument * arg2){

        if(arg2->type == argument::FUNC_CALL){
            call(i, arg2);
        }

        if(arg1->type == argument::VAR){

            if(arg2->type == argument::CONST)
                code(static_cast<int>(i.line_num), ADDI, get_reg(arg1->var), 0, arg2->value);
            if(arg2->type == argument::VAR)
                code(static_cast<int>(i.line_num), ADDI, get_reg(arg1->var), get_reg(arg2->var), 0);
            if(arg2->type == argument::INSTRUCT){
                if(prev != nullptr && prev->operation == ADDA)
                    code(static_cast<int>(i.line_num), LDX, get_reg(arg1->var), get_reg(arg2->value), 0);
                else
                    code(static_cast<int>(i.line_num), ADDI, get_reg(arg1->var), get_reg(arg2->value), 0);
            }

            if(arg2->type == argument::ADDR)
                code(static_cast<int>(i.line_num), ADDI, get_reg(arg1->var), get_reg(arg2->var));

        } else if (arg1->type == argument::INSTRUCT){ //something like let b[4] <- 7

            if(arg2->type == argument::CONST)
                code(static_cast<int>(i.line_num), STW, allocate_reg_const(i, arg2->value), get_reg(arg1->value), 0);
            else
                code(static_cast<int>(i.line_num), STW, get_reg(arg2->value), get_reg(arg1->value), 0);

        } else { //something like let z <- b when b is an array

        }
    }

    void adda(instruction &i, argument * arg1, argument * arg2){
        int store_reg = get_reg(arg1->var);
        int offset_reg = get_reg(arg2->var);
        code(static_cast<int>(i.line_num), ADD, allocate_reg(), store_reg, offset_reg);
    }

    void cmp(instruction &i, argument * arg1, argument * arg2){
        arithmetic(CMP, CMPI, i, arg1, arg2);
    }

    void add(instruction &i, argument * arg1, argument * arg2){
        arithmetic(ADD, ADDI, i, arg1, arg2);
    }
    void mul(instruction &i, argument * arg1, argument * arg2){
        arithmetic(MUL, MULI, i, arg1, arg2);
    }
    void sub(instruction &i, argument * arg1, argument * arg2){
        arithmetic(SUB, SUBI, i, arg1, arg2);
    }
    void div(instruction &i, argument * arg1, argument * arg2){
        arithmetic(DIV, DIVI, i, arg1, arg2);
    }

    void code(int line_num, int first){
        code(line_num, first, -1, -1, -1);
    }
    void code(int line_num, int first, int second){
        code(line_num, first, second, -1, -1);
    }
    void code(int line_num, int first, int second, int third){
        code(line_num, first, second, third, -1);
    }
    void code(int line_num, int first, int second, int third, int fourth){
        M[_instr][0] = line_num;
        M[_instr][1] = first;
        M[_instr][2] = second;
        M[_instr][3] = third;
        M[_instr][4] = fourth;
        _instr++;
    }

    int NOP(){
        code(-1, ADDI, 0, 0, 0);
        return _instr - 1;
    }

    /** Writes M to file with .241 extension. */
    void write(const std::string &file_num){
        std::ofstream outfile;
        outfile.open("output" + file_num + ".241");
        int i = 0;
        for(; i < MemSize && !(M[i][1] == RET && M[i][2] == 0); i++){
            outfile << M[i][1] << " ";
            outfile << M[i][2] << " ";
            outfile << M[i][3] << " ";
            outfile << M[i][4] << "\n";
        }

        outfile << M[i][1] << " ";
        outfile << M[i][2] << " ";
        outfile << M[i][3] << " ";
        outfile << M[i][4] << "\n";

        outfile.close();
    }

    int assign_reg(instruction &i, argument * arg){

        if(arg->type == argument::VAR || arg->type == argument::CONST){
            std::string input = arg->type == argument::VAR ? arg->var : std::to_string(arg->value);
            int reg = get_reg(input);
            if(!reg){
                reg = allocate_reg(arg->var);
                code(static_cast<int>(i.line_num), ADDI, reg, 0, arg->value);
            }
            return reg;
        }

        //otherwise, must be an instruction
        return get_reg(arg->value);
    }

    int get_reg(const std::string &var){
        for(int i = 1; i < 28; i++)
            if(regs[i] != nullptr && *regs[i] == var)
                return i;
        return 0;
    }

    int get_reg(int line_num){
        int copy = _instr;
        while (--copy >= 0) //search upwards, find instruction with line number
            if(M[copy][0] == line_num)
                return M[copy][2];
        return 0;
    }

    //runtime version
    int allocate_reg(){
        return allocate_reg("RUNTIME");
    }

    int allocate_reg_const(instruction &i, int value){
        int reg = get_reg(std::to_string(value));
        if(reg)
            return reg;
        for(int x = 1; x < 28; x++) //regs 1-27 are available
            if(regs[x] == nullptr){
                regs[x] = new std::string(std::to_string(value));
                code(static_cast<int>(i.line_num), ADDI, x, 0, value);
                return x;
            }
        return 0; //false if we cant find any
    }

    //compile time version
    int allocate_reg(const std::string &var){
        //check for bounds later, stack things, etc.

        //search for open reg space
        for(int i = 1; i < 28; i++) //regs 1-27 are available
            if(regs[i] == nullptr){
                regs[i] = new std::string(var);
                return i;
            }
        return 0; //false if we cant find any
    }

    void clear_variable_registers(){
        for(int i = 1; i < 28; i++)
            if(regs[i] != nullptr && isdigit(regs[i]->front()))
                clear_reg(*regs[i]);
    }

    void clear_reg(const std::string & var){
        for(int i = 1; i < 28; i++) //regs 1-27 are available
            if(regs[i] != nullptr && *regs[i] == var)
                regs[i] = nullptr;
    }

};



#endif //INC_241COMPILER_DLX_BUILDER_H
