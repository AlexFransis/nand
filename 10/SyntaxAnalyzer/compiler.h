#ifndef GUARD_compiler_h
#define GUARD_compiler_h

#include <string>
#include <vector>
#include <memory>
#include "tokenizer.h"
#include "ast_node.h"
#include "vm_emitter.h"
#include "symbol_table.h"

class Compiler
{
private:
        std::vector<Token>::const_iterator m_curr_token;
        VMEmitter m_vme;
        SymbolTable m_st;
        AstNode m_curr_node;
        std::vector<std::string> m_vm_code;

        void debug(const std::string &context);
        void debug(const SymbolTable &st);
        inline void advance() { m_curr_token++; }
        inline std::string current_value() { return m_curr_token->value; }
        inline TOKEN_TYPE current_type() { return m_curr_token->type; }
        inline std::string lookahead_value() { return (m_curr_token + 1)->value; }
        inline std::unique_ptr<AstNode> make_node() { return std::make_unique<AstNode>(AstNode{.token_type = current_type(),
                                                                                               .terminal_value = current_value()}); }

        // TRAVERSE AST TO CONSTRUCT SYMBOL TABLE AND EMIT VM CODE
        void traverse_class(const std::unique_ptr<AstNode> &root);
        void traverse_class_var_dec(const std::unique_ptr<AstNode> &node);
        void traverse_subroutine_dec(const std::unique_ptr<AstNode> &node);
        void traverse_parameter_list(const std::unique_ptr<AstNode> &node);
        void traverse_subroutine_body(const std::unique_ptr<AstNode> &node);
        void traverse_var_dec(const std::unique_ptr<AstNode> &node);
        void traverse_statements(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx);
        void traverse_let(const std::unique_ptr<AstNode> &node);
        void traverse_return(const std::unique_ptr<AstNode> &node);
        void traverse_expression(const std::unique_ptr<AstNode> &node);
        void traverse_term(const std::unique_ptr<AstNode> &node);
        int traverse_expression_list(const std::unique_ptr<AstNode> &node);
        void traverse_if(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx);
        void traverse_while(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx);


        // PROGRAM STRUCTURE
        std::unique_ptr<AstNode> compile_class();
        std::unique_ptr<AstNode> compile_class_var_dec();
        std::unique_ptr<AstNode> compile_subroutine_dec();
        std::unique_ptr<AstNode> compile_parameter_list();
        std::unique_ptr<AstNode> compile_subroutine_body();
        std::unique_ptr<AstNode> compile_var_dec();

        // STATEMTENTS
        std::unique_ptr<AstNode> compile_statements();
        std::unique_ptr<AstNode> compile_let();
        std::unique_ptr<AstNode> compile_while();
        std::unique_ptr<AstNode> compile_if();
        std::unique_ptr<AstNode> compile_do();
        std::unique_ptr<AstNode> compile_return();

        // EXPRESSIONS
        std::unique_ptr<AstNode> compile_expression();
        std::unique_ptr<AstNode> compile_term();
        std::unique_ptr<AstNode> compile_subroutine_call();
        std::unique_ptr<AstNode> compile_expression_list();

public:
        Compiler();
        Compiler(const std::vector<Token> &tokens);

        std::unique_ptr<AstNode> generate_ast();
        std::vector<std::string> generate_vm_code(const std::unique_ptr<AstNode> &ast);
};

#endif
