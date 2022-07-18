#include "compiler.h"
#include <memory>


Compiler::Compiler(const std::vector<Token> &tokens)
{
        m_curr_token = tokens.begin();
}

void Compiler::advance()
{
        m_curr_token++;
}

std::unique_ptr<AstNode> Compiler::compile()
{
        return compile_class();
}


std::unique_ptr<AstNode> Compiler::compile_class()
{
        /* 'class' className '{' classVarDec* subroutineDec* '}' */
        std::unique_ptr<AstNode> compiled_class = std::make_unique<AstNode>(AstNode { "class" });

        // 'class'
        compiled_class->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
        advance();

        // className
        compiled_class->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
        advance();

        // '{'
        compiled_class->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
        advance();

        // classVarDec*
        while (m_curr_token->value == "static" || m_curr_token->value == "field") {
                compiled_class->children.push_back(compile_class_var_dec());
                advance();
        }

        // subroutineDec*
        while (m_curr_token->value == "constructor" || m_curr_token->value == "function" || m_curr_token->value == "method") {
                compiled_class->children.push_back(compile_subroutine_dec());
                advance();
        }

        // '}'
        compiled_class->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
        advance();

        return compiled_class;
}

std::unique_ptr<AstNode> Compiler::compile_class_var_dec()
{
        /* ('static' | 'field') type varName (',' varName)* ';' */
        std::unique_ptr<AstNode> class_var_dec = std::make_unique<AstNode>(AstNode { "classVarDec" });

        // 'static' | 'field'
        class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
        advance();

        // type
        class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
        advance();

        // varName
        class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
        advance();

        // (, varName)*
        while (m_curr_token->value != ";") {
                class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
                advance();
                class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));
                advance();
        }

        // ';'
        class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value }));

        return class_var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_subroutine_dec()
{
        std::unique_ptr<AstNode> subroutine_dec = std::make_unique<AstNode>(AstNode { "subroutineDec" });

        /* ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody */

        return subroutine_dec;
}
