#ifndef GUARD_vm_emitter_h
#define GUARD_vm_emitter_h

#include "ast_node.h"
#include "symbol_table.h"
#include <vector>
#include <string>
#include <memory>

enum class SEGMENT {
        ARGUMENT,
        LOCAL,
        STATIC,
        CONSTANT,
        THIS,
        THAT,
        POINTER,
        TEMP
};

enum class COMMAND {
        ADD,
        SUB,
        NEG,
        EQ,
        GT,
        LT,
        AND,
        OR,
        NOT
};

enum class PUSH_POP {
        PUSH,
        POP
};

class VMEmitter {
public:
        void emit_push(const SEGMENT &segment, int i, std::vector<std::string> &out_vm_commands);
        void emit_push(const SCOPE &scope, int i, std::vector<std::string> &out_vm_commands);
        void emit_pop(const SEGMENT &segment, int i, std::vector<std::string> &out_vm_commands);
        void emit_pop(const SCOPE &scope, int i, std::vector<std::string> &out_vm_commands);
        void emit_arithmetic(const COMMAND &command, std::vector<std::string> &out_vm_commands);
        void emit_label(const std::string &label, std::vector<std::string> &out_vm_commands);
        void emit_goto(const std::string &label, std::vector<std::string> &out_vm_commands);
        void emit_if(const std::string &label, std::vector<std::string> &out_vm_commands);
        void emit_call(const std::string &name, int n_args, std::vector<std::string> &out_vm_commands);
        void emit_function(const std::string &name, int n_locals, std::vector<std::string> &out_vm_commands);
        void emit_return(std::vector<std::string> &out_vm_commands);
};


#endif
