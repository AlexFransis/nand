#include "compiler.h"
#include <algorithm>
#include <memory>


Compiler::Compiler(const std::vector<Token> &tokens)
{
        m_curr_token = tokens.begin();
}

void Compiler::advance()
{
        m_curr_token++;
}

std::unique_ptr<AstNode> Compiler::make_node()
{
        return std::make_unique<AstNode>(AstNode { m_curr_token->type, m_curr_token->value });
}

void add_child(const std::unique_ptr<AstNode> &node, const std::unique_ptr<AstNode> &child)
{
        node->children.push_back(std::move(child));
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
        add_child(compiled_class, make_node());
        advance();

        // className
        add_child(compiled_class, make_node());
        advance();

        // '{'
        add_child(compiled_class, make_node());
        advance();

        // classVarDec*
        while (m_curr_token->value == "static" || m_curr_token->value == "field") {
                add_child(compiled_class, compile_class_var_dec());
                advance();
        }

        // subroutineDec*
        while (m_curr_token->value == "constructor" || m_curr_token->value == "function" || m_curr_token->value == "method") {
                add_child(compiled_class, compile_subroutine_dec());
                advance();
        }

        // '}'
        add_child(compiled_class, make_node());
        advance();

        return compiled_class;
}

std::unique_ptr<AstNode> Compiler::compile_class_var_dec()
{
        /* ('static' | 'field') type varName (',' varName)* ';' */
        std::unique_ptr<AstNode> class_var_dec = std::make_unique<AstNode>(AstNode { "classVarDec" });

        // 'static' | 'field'
        add_child(class_var_dec, make_node());
        advance();

        // type
        add_child(class_var_dec, make_node());
        advance();

        // varName
        add_child(class_var_dec, make_node());
        advance();

        // (, varName)*
        while (m_curr_token->value != ";") {
                add_child(class_var_dec, make_node());
                advance();
                add_child(class_var_dec, make_node());
                advance();
        }

        // ';'
        add_child(class_var_dec, make_node());

        return class_var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_subroutine_dec()
{
        /* ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody */
        std::unique_ptr<AstNode> subroutine_dec = std::make_unique<AstNode>(AstNode { "subroutineDec" });

        // ('constructor' | 'function' | 'method')
        add_child(subroutine_dec, make_node());
        advance();

        // ('void' | type)
        add_child(subroutine_dec, make_node());
        advance();

        // subroutineName
        add_child(subroutine_dec, make_node());
        advance();

        // '('
        add_child(subroutine_dec, make_node());
        advance();

        // parameterList
        if (m_curr_token->value != ")") {
                add_child(subroutine_dec, compile_parameter_list());
        }

        // ')'
        add_child(subroutine_dec, make_node());
        advance();

        // subroutineBody
        add_child(subroutine_dec, compile_subroutine_body());
        advance();

        return subroutine_dec;
}

std::unique_ptr<AstNode> Compiler::compile_parameter_list()
{
        /* ( (type varName) (',' type varName)*)? */
        std::unique_ptr<AstNode> parameter_list = std::make_unique<AstNode>(AstNode { "parameterList" });

        // type
        add_child(parameter_list, make_node());
        advance();

        // varName
        add_child(parameter_list, make_node());
        advance();

        while (m_curr_token->value != ")") {
                // ','
                add_child(parameter_list, make_node());
                advance();
                // type
                add_child(parameter_list, make_node());
                advance();
                // varName
                add_child(parameter_list, make_node());
                advance();
        }

        return parameter_list;
}

std::unique_ptr<AstNode> Compiler::compile_subroutine_body()
{
        /* '{' varDec* statements '}' */
        std::unique_ptr<AstNode> subroutine_body = std::make_unique<AstNode>(AstNode { "subroutineBody" });

        // '{'
        add_child(subroutine_body, make_node());
        advance();

        // varDec*
        add_child(subroutine_body, compile_var_dec());
        advance();

        return subroutine_body;
}

std::unique_ptr<AstNode> Compiler::compile_var_dec()
{
        /* 'var' type varName (',' varName)* ';' */
        std::unique_ptr<AstNode> var_dec = std::make_unique<AstNode>(AstNode { "varDec" });



        return var_dec;
}
