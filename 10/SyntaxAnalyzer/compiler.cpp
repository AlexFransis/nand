#include "compiler.h"
#include "compiler.h"
#include <algorithm>
#include <memory>
#include <cassert>


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

std::unique_ptr<AstNode> Compiler::compile()
{
        return compile_class();
}


std::unique_ptr<AstNode> Compiler::compile_class()
{
        // 'class' className '{' classVarDec* subroutineDec* '}'
        std::unique_ptr<AstNode> compiled_class = std::make_unique<AstNode>(AstNode { "class" });

        // 'class'
        assert(m_curr_token->value == "class");
        compiled_class->children.push_back(make_node());
        advance();

        // className
        compiled_class->children.push_back(make_node());
        advance();

        // '{'
        assert(m_curr_token->value == "{");
        compiled_class->children.push_back(make_node());
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
        assert(m_curr_token->value == "}");
        compiled_class->children.push_back(make_node());

        return compiled_class;
}

std::unique_ptr<AstNode> Compiler::compile_class_var_dec()
{
        // ('static' | 'field') type varName (',' varName)* ';'
        std::unique_ptr<AstNode> class_var_dec = std::make_unique<AstNode>(AstNode { "classVarDec" });

        // 'static' | 'field'
        assert(m_curr_token->value == "static" || m_curr_token->value == "field");
        class_var_dec->children.push_back(make_node());
        advance();

        // type
        class_var_dec->children.push_back(make_node());
        advance();

        // varName
        class_var_dec->children.push_back(make_node());
        advance();

        // (, varName)*
        while (m_curr_token->value != ";") {
                // ','
                assert(m_curr_token->value == ",");
                class_var_dec->children.push_back(make_node());
                advance();
                // varName
                class_var_dec->children.push_back(make_node());
                advance();
        }

        // ';'
        assert(m_curr_token->value == ";");
        class_var_dec->children.push_back(make_node());

        return class_var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_subroutine_dec()
{
        // ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
        std::unique_ptr<AstNode> subroutine_dec = std::make_unique<AstNode>(AstNode { "subroutineDec" });

        // ('constructor' | 'function' | 'method')
        assert(m_curr_token->value == "constructor" ||
               m_curr_token->value == "function" ||
               m_curr_token->value == "method");
        subroutine_dec->children.push_back(make_node());
        advance();

        // ('void' | type)
        subroutine_dec->children.push_back(make_node());
        advance();

        // subroutineName
        subroutine_dec->children.push_back(make_node());
        advance();

        // '('
        assert(m_curr_token->value == "(");
        subroutine_dec->children.push_back(make_node());
        advance();

        // parameterList
        if (m_curr_token->value != ")") {
                subroutine_dec->children.push_back(compile_parameter_list());
        }

        // ')'
        assert(m_curr_token->value == ")");
        subroutine_dec->children.push_back(make_node());
        advance();

        // subroutineBody
        subroutine_dec->children.push_back(compile_subroutine_body());

        return subroutine_dec;
}

std::unique_ptr<AstNode> Compiler::compile_parameter_list()
{
        // ( (type varName) (',' type varName)*)?
        std::unique_ptr<AstNode> parameter_list = std::make_unique<AstNode>(AstNode { "parameterList" });

        // type
        parameter_list->children.push_back(make_node());
        advance();

        // varName
        parameter_list->children.push_back(make_node());
        advance();

        while (m_curr_token->value != ")") {
                // ','
                assert(m_curr_token->value == ",");
                parameter_list->children.push_back(make_node());
                advance();
                // type
                parameter_list->children.push_back(make_node());
                advance();
                // varName
                parameter_list->children.push_back(make_node());
                advance();
        }

        return parameter_list;
}

std::unique_ptr<AstNode> Compiler::compile_subroutine_body()
{
        // '{' varDec* statements '}'
        std::unique_ptr<AstNode> subroutine_body = std::make_unique<AstNode>(AstNode { "subroutineBody" });

        // '{'
        assert(m_curr_token->value == "{");
        subroutine_body->children.push_back(make_node());
        advance();

        // varDec*
        while (m_curr_token->value == "var") {
                subroutine_body->children.push_back(compile_var_dec());
                advance();
        }

        // statements
        if (m_curr_token->value != "}") {
                subroutine_body->children.push_back(compile_statements());
                advance();
        }

        // '}'
        assert(m_curr_token->value == "}");
        subroutine_body->children.push_back(make_node());

        return subroutine_body;
}

std::unique_ptr<AstNode> Compiler::compile_var_dec()
{
        // 'var' type varName (',' varName)* ';'
        std::unique_ptr<AstNode> var_dec = std::make_unique<AstNode>(AstNode { "varDec" });

        // 'var'
        assert(m_curr_token->value == "var");
        var_dec->children.push_back(make_node());
        advance();

        // type
        var_dec->children.push_back(make_node());
        advance();

        // varName
        var_dec->children.push_back(make_node());
        advance();

        // varName
        while (m_curr_token->value != ";") {
                // ','
                assert(m_curr_token->value == ",");
                var_dec->children.push_back(make_node());
                advance();
                // varName
                var_dec->children.push_back(make_node());
                advance();
        }

        // ';'
        assert(m_curr_token->value == ";");
        var_dec->children.push_back(make_node());

        return var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_statements()
{
        // (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
        std::unique_ptr<AstNode> statements = std::make_unique<AstNode>(AstNode { "statements" });

        while (m_curr_token->value != "}") {
                if (m_curr_token->value == "let") {
                        statements->children.push_back(compile_let());
                        advance();
                }

                if (m_curr_token->value == "if") {
                        statements->children.push_back(compile_if());
                        advance();
                }

                if (m_curr_token->value == "while") {
                        statements->children.push_back(compile_while());
                        advance();
                }

                if (m_curr_token->value == "do") {
                        statements->children.push_back(compile_do());
                        advance();
                }

                if (m_curr_token->value == "return") {
                        statements->children.push_back(compile_return());
                        advance();
                }
        }

        return statements;
}


std::unique_ptr<AstNode> Compiler::compile_let()
{
        // 'let' varName ('[' expression ']')? '=' expression ';'
        std::unique_ptr<AstNode> let_statement = std::make_unique<AstNode>(AstNode { "letStatement" });

        // 'let'
        assert(m_curr_token->value == "let");
        let_statement->children.push_back(make_node());
        advance();

        // varName
        let_statement->children.push_back(make_node());
        advance();

        if (m_curr_token->value != "=") {
                // '['
                assert(m_curr_token->value == "[");
                let_statement->children.push_back(make_node());
                advance();

                // expression
                let_statement->children.push_back(compile_expression());
                advance();

                // ']'
                assert(m_curr_token->value == "[");
                let_statement->children.push_back(make_node());
                advance();
        }

        assert(m_curr_token->value == ";");
        let_statement->children.push_back(make_node());

        return let_statement;
}

std::unique_ptr<AstNode> Compiler::compile_if()
{
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { "expression" });
        return expression;
}

std::unique_ptr<AstNode> Compiler::compile_while()
{
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { "expression" });
        return expression;
}

std::unique_ptr<AstNode> Compiler::compile_do()
{
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { "expression" });
        return expression;
}

std::unique_ptr<AstNode> Compiler::compile_return()
{
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { "expression" });
        return expression;
}

std::unique_ptr<AstNode> Compiler::compile_expression()
{
        // term (op term)*
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { "expression" });

        // term
        expression->children.push_back(compile_term());
        advance();

        // (op term)*
        while (m_curr_token->value == "+" || m_curr_token->value == "-" || m_curr_token->value == "*" ||
               m_curr_token->value == "/" || m_curr_token->value == "&" || m_curr_token->value == "|" ||
               m_curr_token->value == "<" || m_curr_token->value == ">" || m_curr_token->value == "=") {
                // op
                expression->children.push_back(make_node());
                advance();

                // term
                expression->children.push_back(compile_term());
                advance();
        }

        return expression;
}

std::unique_ptr<AstNode> Compiler::compile_term()
{
        // intConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' |
        // subroutineCall | '(' expression ')' | unaryOp term
        std::unique_ptr<AstNode> term = std::make_unique<AstNode>(AstNode { "term" });

        // keywordConstant
        if (m_curr_token->value == "true" || m_curr_token->value == "false" ||
            m_curr_token->value == "null" || m_curr_token->value == "this") {
                term->children.push_back(make_node());
                return term;
        }

        // stringConstant | intConstant
        if (m_curr_token->type == "INTEGER_CONST" || m_curr_token->type == "STRING_CONST") {
                term->children.push_back(make_node());
                return term;
        }

        // unaryOp term
        if (m_curr_token->value == "-" || m_curr_token->value == "~") {
                // unaryOp
                term->children.push_back(make_node());
                advance();

                // term
                term->children.push_back(make_node());

                return term;
        }

        // '(' expression ')'
        if (m_curr_token->value == "(") {
                // '('
                term->children.push_back(make_node());
                advance();

                // expression
                term->children.push_back(compile_expression());
                advance();

                // ')'
                assert(m_curr_token->value == ")");
                term->children.push_back(make_node());

                return term;
        }

        // lookahead to determine if compile expression or subroutineCall
        std::string lookahead = (m_curr_token+1)->value;

        // varName '[' expression ']'
        if (lookahead == "[") {
                // varName
                term->children.push_back(make_node());
                advance();

                // '['
                term->children.push_back(make_node());
                advance();

                // expression
                term->children.push_back(compile_expression());
                advance();

                // ']'
                assert(m_curr_token->value == "]");
                term->children.push_back(make_node());

                return term;
        }

        // subroutineCall
        if (lookahead == "(") {
                term->children.push_back(compile_subroutine_call());

                return term;
        }

        return term;
}

std::unique_ptr<AstNode> Compiler::compile_subroutine_call()
{
        // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
        std::unique_ptr<AstNode> subroutine_call = std::make_unique<AstNode>(AstNode { "subroutineCall" });

        // subroutineName | className | varName
        subroutine_call->children.push_back(make_node());
        advance();

        if (m_curr_token->value == "(") {
                // '('
                subroutine_call->children.push_back(make_node());
                advance();

                // expressionList
                subroutine_call->children.push_back(compile_expression_list());
                advance();

                // ')'
                assert(m_curr_token->value == ")");
                subroutine_call->children.push_back(make_node());

                return subroutine_call;
        }

        if (m_curr_token->value == ".") {
                // '.'
                subroutine_call->children.push_back(make_node());
                advance();

                // subroutineName
                subroutine_call->children.push_back(make_node());
                advance();

                // '('
                subroutine_call->children.push_back(make_node());
                advance();

                // expressionList
                subroutine_call->children.push_back(compile_expression_list());
                advance();

                // ')'
                assert(m_curr_token->value == ")");
                subroutine_call->children.push_back(make_node());

                return subroutine_call;
        }

        return subroutine_call;
}

std::unique_ptr<AstNode> Compiler::compile_expression_list()
{
        // (expression (',' expression)* )?
        std::unique_ptr<AstNode> expression_list = std::make_unique<AstNode>(AstNode { "expressionList" });

        // expression
        expression_list->children.push_back(compile_expression());
        advance();

        while (m_curr_token->value == ",") {
                // ','
                expression_list->children.push_back(make_node());
                advance();

                // expression
                expression_list->children.push_back(compile_expression());
                advance();

        }

        return expression_list;
}
