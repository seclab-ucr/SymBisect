//
// Created by yu on 5/2/21.
//

#ifndef INC_LLVM_RELATED_H
#define INC_LLVM_RELATED_H

#include "basic.h"

llvm::BasicBlock *get_real_basic_block(llvm::BasicBlock *b);

llvm::BasicBlock *get_final_basic_block(llvm::BasicBlock *b);

std::string get_file_name(llvm::Function *f);

std::string get_real_function_name(llvm::Function *f);

std::string get_structure_name(std::string name);

void dump_inst(llvm::Instruction *inst);

std::string dump_inst_booltin(llvm::Instruction *inst, std::string kernelversion);
std::string dump_inst_sourceinfo(llvm::Instruction *inst);

std::string real_inst_str(std::string str);

/// Compute the true target of a function call, resolving LLVM aliases
/// and bitcasts.
llvm::Function *get_target_function(llvm::Value *calledVal);

#define yhao_llvm_print(type, empty, out, print, str)   \
    if ((type) >= DEBUG_LEVEL) {                        \
        if ((empty) == 1) {                             \
            (str) = "";                                 \
        }                                               \
        llvm::raw_string_ostream dump(str);             \
        print(dump);                                    \
        if ((out) == 1) {                               \
            yhao_log(type, str);                        \
        }                                               \
    }

#define yhao_add(print, str)  yhao_llvm_print(4, 0, 0, print, str)
#define yhao_print(print, str)  yhao_llvm_print(4, 1, 0, print, str)
#define yhao_dump(type, print, str)  yhao_llvm_print(type, 1, 1, print, str)
#define yhao_dump_debug(print, str)  yhao_dump(0, print, str)
#define yhao_dump_add(type, print, str)  yhao_llvm_print(type, 0, 1, print, str)
#define yhao_dump_add_debug(print, str)  yhao_dump_add(0, print, str)

// strID: Path-NameFunction-NoBB-NoInst
std::string function_to_strID(llvm::Function *f);

std::string basicblock_to_strID(llvm::BasicBlock *b);

std::string inst_to_strID(llvm::Instruction *inst);

llvm::Function *strID_to_function(llvm::Module *m, const std::string &str);

llvm::BasicBlock *strID_to_basicblock(llvm::Module *m, const std::string &str);

llvm::Instruction *strID_to_inst(llvm::Module *m, const std::string &str);

#endif //INC_LLVM_RELATED_H
