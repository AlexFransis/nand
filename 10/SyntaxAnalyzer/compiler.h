#ifndef GUARD_compiler_h
#define GUARD_compiler_h

#include <string>
#include <vector>
#include <memory>
#include "ast_node.h"
#include "vm_emitter.h"
#include "symbol_table.h"

class Compiler
{
private:
        VMEmitter m_vme;
        SymbolTable m_st;
        std::vector<std::string> m_vm_code;

        void debug(const SymbolTable &st);

        // COMPILE AST TO CONSTRUCT SYMBOL TABLE AND EMIT VM CODE
        void compile_class(const std::unique_ptr<AstNode> &root);
        void compile_class_var_dec(const std::unique_ptr<AstNode> &node);
        void compile_subroutine_dec(const std::unique_ptr<AstNode> &node);
        void compile_parameter_list(const std::unique_ptr<AstNode> &node);
        void compile_subroutine_body(const std::unique_ptr<AstNode> &node, std::string subroutine_type);
        void compile_var_dec(const std::unique_ptr<AstNode> &node);

        void compile_statements(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx);
        void compile_let(const std::unique_ptr<AstNode> &node);
        void compile_return(const std::unique_ptr<AstNode> &node);
        void compile_if(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx);
        void compile_while(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx);

        void compile_expression(const std::unique_ptr<AstNode> &node);
        void compile_term(const std::unique_ptr<AstNode> &node);
        int compile_expression_list(const std::unique_ptr<AstNode> &node);

public:
        Compiler();

        std::vector<std::string> generate_vm_code(const std::unique_ptr<AstNode> &ast);
};

#endif
