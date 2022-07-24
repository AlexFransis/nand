#include "compiler.h"
#include <algorithm>
#include <memory>
#include <iostream>
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

std::string Compiler::lookahead_value()
{
        return (m_curr_token+1)->value;
}

std::unique_ptr<AstNode> Compiler::compile()
{
        return compile_class();
}

void Compiler::debug(const std::string &context)
{
        std::cout << "COMPILING " << context << " ==> "  << m_curr_token->value << std::endl;
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

        if (lookahead_value() == "static" || lookahead_value() == "field") {
                do {
                        advance();
                        compiled_class->children.push_back(compile_class_var_dec());
                } while(lookahead_value() == "static" || lookahead_value() == "field");
        }

        if (lookahead_value() == "constructor" || lookahead_value() == "function" || lookahead_value() == "method") {
                do {
                        advance();
                        compiled_class->children.push_back(compile_subroutine_dec());
                } while(lookahead_value() == "constructor" || lookahead_value() == "function" || lookahead_value() == "method");
        }

        advance();

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

        if (lookahead_value() == ",") {
                do {
                        advance();
                        // ','
                        assert(m_curr_token->value == ",");
                        class_var_dec->children.push_back(make_node());
                        advance();

                        // varName
                        class_var_dec->children.push_back(make_node());
                } while (lookahead_value() == ",");
        }

        advance();
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

        if (lookahead_value() != ")") {
                advance();
                subroutine_dec->children.push_back(compile_parameter_list());
        }
        advance();

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

        if (lookahead_value() == ",") {
                do {
                        advance();
                        // ','
                        assert(m_curr_token->value == ",");
                        parameter_list->children.push_back(make_node());
                        advance();
                        // type
                        parameter_list->children.push_back(make_node());
                        advance();
                        // varName
                        parameter_list->children.push_back(make_node());
                } while (lookahead_value() == ",");
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

        // varDec*
        if (lookahead_value() == "var") {
                advance();
                subroutine_body->children.push_back(compile_var_dec());
        }

        // statements
        if (lookahead_value() == "let" ||
            lookahead_value() == "if" ||
            lookahead_value() == "while" ||
            lookahead_value() == "do" ||
            lookahead_value() == "return") {
                subroutine_body->children.push_back(compile_statements());
        }

        advance();
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
        if (lookahead_value() == ",") {
                do {
                        advance();
                        // ','
                        assert(m_curr_token->value == ",");
                        var_dec->children.push_back(make_node());
                        advance();
                        // varName
                        var_dec->children.push_back(make_node());
                } while (lookahead_value() == ",");
        }

        advance();
        // ';'
        assert(m_curr_token->value == ";");
        var_dec->children.push_back(make_node());

        return var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_statements()
{
        // (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
        std::unique_ptr<AstNode> statements = std::make_unique<AstNode>(AstNode { "statements" });

        do {
                advance();

                if (m_curr_token->value == "let") {
                        statements->children.push_back(compile_let());
                        continue;
                }

                if (m_curr_token->value == "if") {
                        statements->children.push_back(compile_if());
                        continue;
                }

                if (m_curr_token->value == "while") {
                        statements->children.push_back(compile_while());
                        continue;
                }

                if (m_curr_token->value == "do") {
                        statements->children.push_back(compile_do());
                        continue;
                }

                if (m_curr_token->value == "return") {
                        statements->children.push_back(compile_return());
                        continue;
                }

        }
        while (lookahead_value() == "do" || lookahead_value() == "let" || lookahead_value() == "while" ||
               lookahead_value() == "if" || lookahead_value() == "return");

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
                assert(m_curr_token->value == "]");
                let_statement->children.push_back(make_node());
                advance();
        }

        // '='
        assert(m_curr_token->value == "=");
        let_statement->children.push_back(make_node());
        advance();

        // expression
        let_statement->children.push_back(compile_expression());
        advance();

        // ';'
        assert(m_curr_token->value == ";");
        let_statement->children.push_back(make_node());

        return let_statement;
}

std::unique_ptr<AstNode> Compiler::compile_if()
{
        // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
        std::unique_ptr<AstNode> if_statement = std::make_unique<AstNode>(AstNode { "ifStatement" });

        // 'if'
        if_statement->children.push_back(make_node());
        advance();

        // '('
        assert(m_curr_token->value == "(");
        if_statement->children.push_back(make_node());
        advance();

        // expression
        if_statement->children.push_back(compile_expression());
        advance();

        // ')'
        assert(m_curr_token->value == ")");
        if_statement->children.push_back(make_node());
        advance();

        // '{'
        assert(m_curr_token->value == "{");
        if_statement->children.push_back(make_node());

        // statements
        if (lookahead_value() == "let" ||
            lookahead_value() == "if" ||
            lookahead_value() == "while" ||
            lookahead_value() == "do" ||
            lookahead_value() == "return") {
                if_statement->children.push_back(compile_statements());
        }
        advance();

        // '}'
        assert(m_curr_token->value == "}");
        if_statement->children.push_back(make_node());

        if (lookahead_value() == "else") {
                advance();

                // else
                assert(m_curr_token->value == "else");
                if_statement->children.push_back(make_node());
                advance();

                // '{'
                assert(m_curr_token->value == "{");
                if_statement->children.push_back(make_node());

                // statements
                if (lookahead_value() == "let" ||
                    lookahead_value() == "if" ||
                    lookahead_value() == "while" ||
                    lookahead_value() == "do" ||
                    lookahead_value() == "return") {
                        if_statement->children.push_back(compile_statements());
                }
                advance();

                // '}'
                assert(m_curr_token->value == "}");
                if_statement->children.push_back(make_node());
        }

        return if_statement;
}

std::unique_ptr<AstNode> Compiler::compile_while()
{
        // 'while' '(' expression ')' '{' statements '}'
        std::unique_ptr<AstNode> while_statement = std::make_unique<AstNode>(AstNode { "whileStatement" });

        // 'while'
        while_statement->children.push_back(make_node());
        advance();

        // '('
        while_statement->children.push_back(make_node());
        advance();

        // 'expression'
        while_statement->children.push_back(compile_expression());
        advance();

        // ')'
        assert(m_curr_token->value == ")");
        while_statement->children.push_back(make_node());
        advance();

        // '{'
        assert(m_curr_token->value == "{");
        while_statement->children.push_back(make_node());

        // 'statements'
        if (lookahead_value() == "let" ||
            lookahead_value() == "if" ||
            lookahead_value() == "while" ||
            lookahead_value() == "do" ||
            lookahead_value() == "return") {
                while_statement->children.push_back(compile_statements());
        }
        advance();

        // '}'
        assert(m_curr_token->value == "}");
        while_statement->children.push_back(make_node());

        return while_statement;
}

std::unique_ptr<AstNode> Compiler::compile_do()
{
        // 'do' subroutineCall ';'
        std::unique_ptr<AstNode> do_statement = std::make_unique<AstNode>(AstNode { "doStatement" });

        // 'do'
        do_statement->children.push_back(make_node());
        advance();

        // subroutineCall
        do_statement->children.push_back(compile_subroutine_call());
        advance();

        // ';'
        assert(m_curr_token->value == ";");
        do_statement->children.push_back(make_node());

        return do_statement;
}

std::unique_ptr<AstNode> Compiler::compile_return()
{
        // 'return' expression? ';'
        std::unique_ptr<AstNode> return_statement = std::make_unique<AstNode>(AstNode { "returnStatement" });

        // 'return'
        return_statement->children.push_back(make_node());
        advance();

        // expression?
        if (m_curr_token->value != ";") {
                return_statement->children.push_back(compile_expression());
                advance();
        }

        // ';'
        assert(m_curr_token->value == ";");
        return_statement->children.push_back(make_node());

        return return_statement;
}

std::unique_ptr<AstNode> Compiler::compile_expression()
{
        // term (op term)*
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { "expression" });

        // term
        expression->children.push_back(compile_term());

        // lookahead to determine if there are op and terms
        if (lookahead_value() != "+" && lookahead_value() != "-" && lookahead_value() != "*" &&
            lookahead_value() != "/" && lookahead_value() != "&" && lookahead_value() != "|" &&
            lookahead_value() != "<" && lookahead_value() != ">" && lookahead_value() != "=") {
                return expression;
        }

        // (op term)*
        do {
                advance();
                // op
                expression->children.push_back(make_node());
                advance();

                // term
                expression->children.push_back(compile_term());
        } while (lookahead_value() == "+" && lookahead_value() == "-" && lookahead_value() == "*" &&
                 lookahead_value() == "/" && lookahead_value() == "&" && lookahead_value() == "|" &&
                 lookahead_value() == "<" && lookahead_value() == ">" && lookahead_value() == "=");

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

        // lookahead to determine if compile expression or subroutineCall or simply a varName

        // varName '[' expression ']'
        if (lookahead_value() == "[") {
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
        if (lookahead_value() == "(") {
                term->children.push_back(compile_subroutine_call());

                return term;
        }

        // varName
        term->children.push_back(make_node());

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

                if (m_curr_token->value == ")") {
                        subroutine_call->children.push_back(make_node());
                        return subroutine_call;
                }

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

                if (m_curr_token->value == ")") {
                        subroutine_call->children.push_back(make_node());
                        return subroutine_call;
                }

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

        if (lookahead_value() != ",") {
                return expression_list;
        }

        do {
                advance();

                // ','
                assert(m_curr_token->value == ",");
                expression_list->children.push_back(make_node());
                advance();

                // expression
                expression_list->children.push_back(compile_expression());
        } while (lookahead_value() == ",");


        return expression_list;
}
