//
// Created by yuhao on 2/2/22.
//

#ifndef KLEE_LISTENERSERVICE_H
#define KLEE_LISTENERSERVICE_H

#include "../ToolLib/basic.h"
#include "Listener.h"

namespace klee {
    class Executor;
}

namespace kuc {
    class ListenerService {
    private:
        klee::Executor *executor{};
        std::vector<Listener *> listeners;

    public:
        explicit ListenerService(klee::Executor *executor);

        ~ListenerService();

        void pushListener(Listener *listener);

        void removeListener(Listener *listener);

        void removeListener(listener_kind kind);

        Listener *popListener();

        void preparation();

        void beforeRun(klee::ExecutionState &state);

        void beforeExecuteInstruction(klee::ExecutionState &state, klee::KInstruction *ki);

        void afterExecuteInstruction(klee::ExecutionState &state, klee::KInstruction *ki);

        void afterRun(klee::ExecutionState &state);

        bool CallInstruction(klee::ExecutionState &state, klee::KInstruction *ki);

        void executionFailed(klee::ExecutionState &state, klee::KInstruction *ki);
    };
}


#endif //KLEE_LISTENERSERVICE_H
