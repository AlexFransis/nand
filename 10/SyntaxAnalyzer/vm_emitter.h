#ifndef GUARD_vm_emitter_h
#define GUARD_vm_emitter_h

#include "ast_node.h"
#include "symbol_table.h"
#include <vector>
#include <string>
#include <memory>

class VMEmitter {
public:
        std::vector<std::string> emit_push(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_pop(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_arithmetic(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_label(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_goto(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_if(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_calll(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_function(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_return(const std::unique_ptr<AstNode> &node);
        std::vector<std::string> emit_vm_commands(const std::unique_ptr<AstNode> &node);
};


#endif
