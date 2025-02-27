//
// Created by yuhao on 2/3/22.
//

#ifndef KLEE_PATHLISTENER_H
#define KLEE_PATHLISTENER_H

#include "Listener.h"
#include "../ToolLib/json.hpp"
#include "../MLTA/Analyzer.hh"

namespace kuc {
    class PathListener : public Listener {
    public:
        explicit PathListener(klee::Executor *executor);

        ~PathListener() override;

        void beforeRun(klee::ExecutionState &initialState) override;

        void beforeExecuteInstruction(klee::ExecutionState &state, klee::KInstruction *ki) override;

        void afterExecuteInstruction(klee::ExecutionState &state, klee::KInstruction *ki) override;

        void afterRun(klee::ExecutionState &state) override;

        bool CallInstruction(klee::ExecutionState &state, klee::KInstruction *ki) override;

        void executionFailed(klee::ExecutionState &state, klee::KInstruction *ki) override;

        void BB_reachableBBs(BasicBlock * BB);
        bool BB1_reach_BB2(BasicBlock * A, BasicBlock * B);
        bool Isaloop(BasicBlock * A, BasicBlock * B, BasicBlock * C);

        bool OOBWcheck(klee::ExecutionState &state, klee::KInstruction *ki);

    public:
        nlohmann::json config;
        std::set<std::string> target_bbs;
        std::set<std::string> low_priority_bbs;
        std::set<std::string> low_priority_functions;
        //added by zheng
        std::set<std::string> low_priority_lines;
        //once reach, do OOBW check, once positive, halt the execution. In most cases there is only one target line
        std::set<std::string> target_lines;

        std::string temp_call_cond_name = "temp_call_cond";
        uint64_t temp_call_cond_count = 0;
        // yu hao: todo: read function map from json
        //std::map<llvm::CallInst *, std::set<llvm::Function *>> function_map;
        // zheng: indirect call map
        std::map<std::string, std::string> indirectcall_map;
        std::map<std::string, std::set<std::string>> whitelist_map;
        std::map<std::string, std::set<std::string>> BB_reachBBs;
        std::uint32_t looplimit;
        std::string kernelversion;
        std::vector<std::string> Calltrace;
        std::map<std::string, int> low_priority_lines_counter;
        GlobalContext GlobalCtx;
    };
}


#endif //KLEE_PATHLISTENER_H
