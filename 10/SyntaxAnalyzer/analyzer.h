#ifndef GUARD_analyzer_h
#define GUARD_analyzer_h

#include <memory>
#include "ast_node.h"
#include "tokenizer.h"

class Analyzer {
private:
        std::vector<Token>::const_iterator m_curr_token;

        void debug(const std::string &context);
        inline void advance() { m_curr_token++; }
        inline std::string current_value() { return m_curr_token->value; }
        inline TOKEN_TYPE current_type() { return m_curr_token->type; }
        inline std::string lookahead_value() { return (m_curr_token + 1)->value; }
        inline std::unique_ptr<AstNode> make_node() { return std::make_unique<AstNode>(AstNode{.token_type = current_type(),
                                                                                               .terminal_value = current_value()}); }
        // PROGRAM STRUCTURE
        std::unique_ptr<AstNode> analyze_class();
        std::unique_ptr<AstNode> analyze_class_var_dec();
        std::unique_ptr<AstNode> analyze_subroutine_dec();
        std::unique_ptr<AstNode> analyze_parameter_list();
        std::unique_ptr<AstNode> analyze_subroutine_body();
        std::unique_ptr<AstNode> analyze_var_dec();

        // STATEMTENTS
        std::unique_ptr<AstNode> analyze_statements();
        std::unique_ptr<AstNode> analyze_let();
        std::unique_ptr<AstNode> analyze_while();
        std::unique_ptr<AstNode> analyze_if();
        std::unique_ptr<AstNode> analyze_do();
        std::unique_ptr<AstNode> analyze_return();

        // EXPRESSIONS
        std::unique_ptr<AstNode> analyze_expression();
        std::unique_ptr<AstNode> analyze_term();
        std::unique_ptr<AstNode> analyze_subroutine_call();
        std::unique_ptr<AstNode> analyze_expression_list();
public:
        std::unique_ptr<AstNode> generate_ast(const std::vector<Token> &tokens);
};

#endif
