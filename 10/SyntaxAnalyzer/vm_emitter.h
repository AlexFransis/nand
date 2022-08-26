#ifndef GUARD_vm_emitter_h
#define GUARD_vm_emitter_h

#include "ast_node.h"
#include "symbol_table.h"
#include <vector>
#include <string>

class VMEmitter {
private:
        void traverse_ast(const AstNode &root, SymbolTable &st, std::vector<std::string> &out_vm_commands);
        std::vector<std::string> emit_push();
        std::vector<std::string> emit_pop();
        std::vector<std::string> emit_arithmetic();
        std::vector<std::string> emit_label();
        std::vector<std::string> emit_goto();
        std::vector<std::string> emit_if();
        std::vector<std::string> emit_calll();
        std::vector<std::string> emit_function();
        std::vector<std::string> emit_return();

public:
        std::vector<std::string> emit_vm_commands(const AstNode &node);
};


#endif
