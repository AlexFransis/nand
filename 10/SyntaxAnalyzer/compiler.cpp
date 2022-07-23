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
        assert(m_curr_token->value == "class");
        add_child(compiled_class, make_node());
        advance();

        // className
        add_child(compiled_class, make_node());
        advance();

        // '{'
        assert(m_curr_token->value == "{");
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
        assert(m_curr_token->value == "}");
        add_child(compiled_class, make_node());

        return compiled_class;
}

std::unique_ptr<AstNode> Compiler::compile_class_var_dec()
{
        /* ('static' | 'field') type varName (',' varName)* ';' */
        std::unique_ptr<AstNode> class_var_dec = std::make_unique<AstNode>(AstNode { "classVarDec" });

        // 'static' | 'field'
        assert(m_curr_token->value == "static" || m_curr_token->value == "field");
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
                // ','
                assert(m_curr_token->value == ",");
                add_child(class_var_dec, make_node());
                advance();
                // varName
                add_child(class_var_dec, make_node());
                advance();
        }

        // ';'
        assert(m_curr_token->value == ";");
        add_child(class_var_dec, make_node());

        return class_var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_subroutine_dec()
{
        /* ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody */
        std::unique_ptr<AstNode> subroutine_dec = std::make_unique<AstNode>(AstNode { "subroutineDec" });

        // ('constructor' | 'function' | 'method')
        assert(m_curr_token->value == "constructor" ||
               m_curr_token->value == "function" ||
               m_curr_token->value == "method");
        add_child(subroutine_dec, make_node());
        advance();

        // ('void' | type)
        add_child(subroutine_dec, make_node());
        advance();

        // subroutineName
        add_child(subroutine_dec, make_node());
        advance();

        // '('
        assert(m_curr_token->value == "(");
        add_child(subroutine_dec, make_node());
        advance();

        // parameterList
        if (m_curr_token->value != ")") {
                add_child(subroutine_dec, compile_parameter_list());
        }

        // ')'
        assert(m_curr_token->value == ")");
        add_child(subroutine_dec, make_node());
        advance();

        // subroutineBody
        add_child(subroutine_dec, compile_subroutine_body());

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
                assert(m_curr_token->value == ",");
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
        assert(m_curr_token->value == "{");
        add_child(subroutine_body, make_node());
        advance();

        // varDec*
        while (m_curr_token->value == "var") {
                add_child(subroutine_body, compile_var_dec());
                advance();
        }

        // statements
        if (m_curr_token->value != "}") {
                add_child(subroutine_body, compile_statements());
                advance();
        }

        // '}'
        assert(m_curr_token->value == "}");
        add_child(subroutine_body, make_node());

        return subroutine_body;
}

std::unique_ptr<AstNode> Compiler::compile_var_dec()
{
        /* 'var' type varName (',' varName)* ';' */
        std::unique_ptr<AstNode> var_dec = std::make_unique<AstNode>(AstNode { "varDec" });

        // 'var'
        assert(m_curr_token->value == "var");
        add_child(var_dec, make_node());
        advance();

        // type
        add_child(var_dec, make_node());
        advance();

        // varName
        add_child(var_dec, make_node());
        advance();

        // varName
        while (m_curr_token->value != ";") {
                // ','
                assert(m_curr_token->value == ",");
                add_child(var_dec, make_node());
                advance();
                // varName
                add_child(var_dec, make_node());
                advance();
        }

        // ';'
        assert(m_curr_token->value == ";");
        add_child(var_dec, make_node());

        return var_dec;
}

std::unique_ptr<AstNode> Compiler::compile_statements()
{
        /* (letStatement | ifStatement | whileStatement | doStatement | returnStatement)* */
        std::unique_ptr<AstNode> statements = std::make_unique<AstNode>(AstNode { "statements" });

        while (m_curr_token->value != "}") {
                if (m_curr_token->value == "let") {
                        add_child(statements, compile_let());
                        advance();
                }

                if (m_curr_token->value == "if") {
                        add_child(statements, compile_if());
                        advance();
                }

                if (m_curr_token->value == "while") {
                        add_child(statements, compile_while());
                        advance();
                }

                if (m_curr_token->value == "do") {
                        add_child(statements, compile_do());
                        advance();
                }

                if (m_curr_token->value == "return") {
                        add_child(statements, compile_return());
                        advance();
                }
        }

        return statements;
}


std::unique_ptr<AstNode> Compiler::compile_let()
{
        /* 'let' varName ('[' expression ']')? '=' expression ';'  */
        std::unique_ptr<AstNode> let_statement = std::make_unique<AstNode>(AstNode { "letStatement" });

        // 'let'
        assert(m_curr_token->value == "let");
        add_child(let_statement, make_node());
        advance();

        // varName
        add_child(let_statement, make_node());
        advance();

        if (m_curr_token->value != "=") {
                // '['
                assert(m_curr_token->value == "[");
                add_child(let_statement, make_node());
                advance();

                // expression
                add_child(let_statement, compile_expression());
                advance();

                // ']'
                assert(m_curr_token->value == "[");
                add_child(let_statement, make_node());
                advance();
        }

        assert(m_curr_token->value == ";");
        add_child(let_statement, make_node());

        return let_statement;
}

std::unique_ptr<AstNode> Compiler::compile_expression()
{
        /* term (op term)* */
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { "expression" });

        // term
        add_child(expression, compile_term());
        advance();

        // (op term)*
        while (m_curr_token->value == "+" || m_curr_token->value == "-" || m_curr_token->value == "*" ||
               m_curr_token->value == "/" || m_curr_token->value == "&" || m_curr_token->value == "|" ||
               m_curr_token->value == "<" || m_curr_token->value == ">" || m_curr_token->value == "=") {
                // op
                add_child(expression, make_node());
                advance();

                // term
                add_child(expression, compile_term());
                advance();
        }

        return expression;
}

std::unique_ptr<AstNode> Compiler::compile_term()
{
        /* intConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' |
           subroutineCall | '(' expression ')' | unaryOp term
         */
        std::unique_ptr<AstNode> term = std::make_unique<AstNode>(AstNode { "term" });

        // keywordConstant
        if (m_curr_token->value == "true" || m_curr_token->value == "false" ||
            m_curr_token->value == "null" || m_curr_token->value == "this") {
                add_child(term, make_node());
                return term;
        }

        // stringConstant | intConstant
        if (m_curr_token->type == "INTEGER_CONST" || m_curr_token->type == "STRING_CONST") {
                add_child(term, make_node());
                return term;
        }

        // unaryOp term
        if (m_curr_token->value == "-" || m_curr_token->value == "~") {
                // unaryOp
                add_child(term, make_node());
                advance();

                // term
                add_child(term, compile_term());

                return term;
        }

        // '(' expression ')'
        if (m_curr_token->value == "(") {
                // '('
                add_child(term, make_node());
                advance();

                // expression
                add_child(term, compile_expression());
                advance();

                // ')'
                assert(m_curr_token->value == ")");
                add_child(term, make_node());

                return term;
        }

        // lookahead to determine if compile expression or subroutineCall
        std::string lookahead = (m_curr_token+1)->value;

        // varName '[' expression ']'
        if (lookahead == "[") {
                // varName
                add_child(term, make_node());
                advance();

                // '['
                add_child(term, make_node());
                advance();

                // expression
                add_child(term, compile_expression());
                advance();

                // ']'
                assert(m_curr_token->value == "]");
                add_child(term, make_node());

                return term;
        }

        // subroutineCall
        if (lookahead == "(") {
                add_child(term, compile_subroutine_call());

                return term;
        }

        return term;
}
