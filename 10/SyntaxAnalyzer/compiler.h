#ifndef GUARD_compiler_h
#define GUARD_compiler_h

#include <string>
#include <vector>
#include <memory>
#include "tokenizer.h"

struct AstNode {
        std::string type;
        std::string terminal_value;
        std::vector<std::unique_ptr<AstNode>> children;
};


class Compiler {
private:
        std::vector<Token>::const_iterator m_curr_token;

        void 				debug(const std::string &context);
        inline void 			advance() { m_curr_token++; }
        inline std::string 		current_value() { return m_curr_token->value; }
        inline std::string 		current_type() { return m_curr_token->type; }
        inline std::string 		lookahead_value() { return (m_curr_token+1)->value; }
        inline std::unique_ptr<AstNode> make_node() { return std::make_unique<AstNode>(AstNode { current_type(), current_value()}); }

        // PROGRAM STRUCTURE
        std::unique_ptr<AstNode> 	compile_class();
        std::unique_ptr<AstNode> 	compile_class_var_dec();
        std::unique_ptr<AstNode> 	compile_subroutine_dec();
        std::unique_ptr<AstNode> 	compile_parameter_list();
        std::unique_ptr<AstNode> 	compile_subroutine_body();
        std::unique_ptr<AstNode> 	compile_var_dec();

        // STATEMTENTS
        std::unique_ptr<AstNode> 	compile_statements();
        std::unique_ptr<AstNode> 	compile_let();
        std::unique_ptr<AstNode> 	compile_while();
        std::unique_ptr<AstNode> 	compile_if();
        std::unique_ptr<AstNode> 	compile_do();
        std::unique_ptr<AstNode> 	compile_return();

        // EXPRESSIONS
        std::unique_ptr<AstNode> 	compile_expression();
        std::unique_ptr<AstNode> 	compile_term();
        std::unique_ptr<AstNode> 	compile_subroutine_call();
        std::unique_ptr<AstNode> 	compile_expression_list();


public:
        Compiler(const std::vector<Token> &tokens);

        inline std::unique_ptr<AstNode> compile() { return compile_class(); }
};

#endif
