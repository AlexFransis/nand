#include "compiler.h"
#include <algorithm>
#include <memory>
#include <iostream>
#include <cassert>


Compiler::Compiler(const std::vector<Token> &tokens)
{
        m_curr_token = tokens.begin();
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
        assert(current_value() == "class");
        compiled_class->children.push_back(make_node());
        advance();

        // className
        compiled_class->children.push_back(make_node());
        advance();

        // '{'
        assert(current_value() == "{");
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
        assert(current_value() == "}");
        compiled_class->children.push_back(make_node());

        return compiled_class;
}

std::unique_ptr<AstNode> Compiler::compile_class_var_dec()
{
        // ('static' | 'field') type varName (',' varName)* ';'
        std::unique_ptr<AstNode> class_var_dec = std::make_unique<AstNode>(AstNode { "classVarDec" });

        // 'static' | 'field'
        assert(current_value() == "static" || current_value() == "field");
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
                        assert(current_value() == ",");
                        class_var_dec->children.push_back(make_node());
                        advance();

                        // varName
                        class_var_dec->children.push_back(make_node());
                } while (lookahead_value() == ",");
        }

        advance();
        // ';'
        assert(current_value() == ";");
        class_var_dec->children.push_back(make_node());

        return class_var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_subroutine_dec()
{
        // ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
        std::unique_ptr<AstNode> subroutine_dec = std::make_unique<AstNode>(AstNode { "subroutineDec" });

        // ('constructor' | 'function' | 'method')
        assert(current_value() == "constructor" || current_value() == "function" || current_value() == "method");
        subroutine_dec->children.push_back(make_node());
        advance();

        // ('void' | type)
        subroutine_dec->children.push_back(make_node());
        advance();

        // subroutineName
        subroutine_dec->children.push_back(make_node());
        advance();

        // '('
        assert(current_value() == "(");
        subroutine_dec->children.push_back(make_node());

        if (lookahead_value() != ")") {
                advance();
                subroutine_dec->children.push_back(compile_parameter_list());
        }
        advance();

        // ')'
        assert(current_value() == ")");
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
                        assert(current_value() == ",");
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
        assert(current_value() == "{");
        subroutine_body->children.push_back(make_node());

        // varDec*
        if (lookahead_value() == "var") {
                advance();
                subroutine_body->children.push_back(compile_var_dec());
        }

        // statements
        subroutine_body->children.push_back(compile_statements());
        advance();

        // '}'
        assert(current_value() == "}");
        subroutine_body->children.push_back(make_node());

        return subroutine_body;
}

std::unique_ptr<AstNode> Compiler::compile_var_dec()
{
        // 'var' type varName (',' varName)* ';'
        std::unique_ptr<AstNode> var_dec = std::make_unique<AstNode>(AstNode { "varDec" });

        // 'var'
        assert(current_value() == "var");
        var_dec->children.push_back(make_node());
        advance();

        // type
        var_dec->children.push_back(make_node());
        advance();

        // varName
        var_dec->children.push_back(make_node());

        // varName
        if (lookahead_value() == ",") {
                do {
                        advance();
                        // ','
                        assert(current_value() == ",");
                        var_dec->children.push_back(make_node());
                        advance();
                        // varName
                        var_dec->children.push_back(make_node());
                } while (lookahead_value() == ",");
        }

        advance();
        // ';'
        assert(current_value() == ";");
        var_dec->children.push_back(make_node());

        return var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_statements()
{
        // (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
        std::unique_ptr<AstNode> statements = std::make_unique<AstNode>(AstNode { "statements" });

        do {
                advance();

                if (current_value() == "let") {
                        statements->children.push_back(compile_let());
                        continue;
                }

                if (current_value() == "if") {
                        statements->children.push_back(compile_if());
                        continue;
                }

                if (current_value() == "while") {
                        statements->children.push_back(compile_while());
                        continue;
                }

                if (current_value() == "do") {
                        statements->children.push_back(compile_do());
                        continue;
                }

                if (current_value() == "return") {
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
        assert(current_value() == "let");
        let_statement->children.push_back(make_node());
        advance();

        // varName
        let_statement->children.push_back(make_node());
        if (lookahead_value() == "[") {
                advance();

                let_statement->children.push_back(make_node());
                advance();
                // expression
                let_statement->children.push_back(compile_expression());
                advance();

                // ']'
                assert(current_value() == "]");
                let_statement->children.push_back(make_node());
        }
        advance();

        // '='
        assert(current_value() == "=");
        let_statement->children.push_back(make_node());
        advance();

        // expression
        let_statement->children.push_back(compile_expression());
        advance();

        // ';'
        assert(current_value() == ";");
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
        assert(current_value() == "(");
        if_statement->children.push_back(make_node());
        advance();

        // expression
        if_statement->children.push_back(compile_expression());
        advance();

        // ')'
        assert(current_value() == ")");
        if_statement->children.push_back(make_node());
        advance();

        // '{'
        assert(current_value() == "{");
        if_statement->children.push_back(make_node());

        // statements
        if_statement->children.push_back(compile_statements());
        advance();

        // '}'
        assert(current_value() == "}");
        if_statement->children.push_back(make_node());

        if (lookahead_value() == "else") {
                advance();

                // else
                assert(current_value() == "else");
                if_statement->children.push_back(make_node());
                advance();

                // '{'
                assert(current_value() == "{");
                if_statement->children.push_back(make_node());

                // statements
                if_statement->children.push_back(compile_statements());
                advance();

                // '}'
                assert(current_value() == "}");
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
        assert(current_value() == ")");
        while_statement->children.push_back(make_node());
        advance();

        // '{'
        assert(current_value() == "{");
        while_statement->children.push_back(make_node());

        // 'statements'
        while_statement->children.push_back(compile_statements());
        advance();

        // '}'
        assert(current_value() == "}");
        while_statement->children.push_back(make_node());

        return while_statement;
}

std::unique_ptr<AstNode> Compiler::compile_do()
{
        // 'do' subroutineCall ';'
        std::unique_ptr<AstNode> do_statement = std::make_unique<AstNode>(AstNode { "doStatement" });

        // 'do'
        assert(current_value() == "do");
        do_statement->children.push_back(make_node());
        advance();

        // subroutineCall
        do_statement->children.push_back(compile_subroutine_call());
        advance();

        // ';'
        assert(current_value() == ";");
        do_statement->children.push_back(make_node());

        return do_statement;
}

std::unique_ptr<AstNode> Compiler::compile_return()
{
        // 'return' expression? ';'
        std::unique_ptr<AstNode> return_statement = std::make_unique<AstNode>(AstNode { "returnStatement" });

        // 'return'
        assert(current_value() == "return");
        return_statement->children.push_back(make_node());

        // expression?
        if (lookahead_value() != ";") {
                advance();
                return_statement->children.push_back(compile_expression());
        }

        advance();

        // ';'
        assert(current_value() == ";");
        return_statement->children.push_back(make_node());

        return return_statement;
}

std::unique_ptr<AstNode> Compiler::compile_expression()
{
        // term (op term)*
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { "expression" });

        // term
        expression->children.push_back(compile_term());

        // (op term)*
        if (lookahead_value() == "+" && lookahead_value() == "-" && lookahead_value() == "*" &&
            lookahead_value() == "/" && lookahead_value() == "&" && lookahead_value() == "|" &&
            lookahead_value() == "<" && lookahead_value() == ">" && lookahead_value() == "=") {
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
        }

        return expression;
}

std::unique_ptr<AstNode> Compiler::compile_term()
{
        // intConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' |
        // subroutineCall | '(' expression ')' | unaryOp term
        std::unique_ptr<AstNode> term = std::make_unique<AstNode>(AstNode { "term" });

        // keywordConstant
        if (current_value() == "true" || current_value() == "false" ||
            current_value() == "null" || current_value() == "this") {
                term->children.push_back(make_node());
                return term;
        }

        // stringConstant | intConstant
        if (current_type() == "INTEGER_CONST" || current_type() == "STRING_CONST") {
                term->children.push_back(make_node());
                return term;
        }

        // unaryOp term
        if (current_value() == "-" || current_value() == "~") {
                // unaryOp
                term->children.push_back(make_node());
                advance();

                // term
                term->children.push_back(make_node());

                return term;
        }

        // '(' expression ')'
        if (current_value() == "(") {
                // '('
                term->children.push_back(make_node());
                advance();

                // expression
                term->children.push_back(compile_expression());
                advance();

                // ')'
                assert(current_value() == ")");
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
                assert(current_value() == "]");
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

        if (current_value() == "(") {
                // '('
                subroutine_call->children.push_back(make_node());
                advance();

                if (current_value() == ")") {
                        subroutine_call->children.push_back(make_node());
                        return subroutine_call;
                }

                // expressionList
                subroutine_call->children.push_back(compile_expression_list());
                advance();

                // ')'
                assert(current_value() == ")");
                subroutine_call->children.push_back(make_node());

                return subroutine_call;
        }

        if (current_value() == ".") {
                // '.'
                subroutine_call->children.push_back(make_node());
                advance();

                // subroutineName
                subroutine_call->children.push_back(make_node());
                advance();

                // '('
                subroutine_call->children.push_back(make_node());
                advance();

                if (current_value() == ")") {
                        subroutine_call->children.push_back(make_node());
                        return subroutine_call;
                }

                // expressionList
                subroutine_call->children.push_back(compile_expression_list());
                advance();

                // ')'
                assert(current_value() == ")");
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

        if (lookahead_value() == ",") {
                do {
                        advance();

                        // ','
                        assert(current_value() == ",");
                        expression_list->children.push_back(make_node());
                        advance();

                        // expression
                        expression_list->children.push_back(compile_expression());
                } while (lookahead_value() == ",");
        }

        return expression_list;
}
