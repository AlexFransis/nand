#include "compiler.h"
#include "ast_node.h"
#include "tokenizer.h"
#include "vm_emitter.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <memory>
#include <vector>


Compiler::Compiler(){}
Compiler::Compiler(const std::vector<Token> &tokens)
{
        m_curr_token = tokens.begin();
        m_vme = VMEmitter();
        m_st = SymbolTable();
}

std::unique_ptr<AstNode> Compiler::generate_ast()
{
        return compile_class();
}

void Compiler::debug(const std::string &context)
{
        std::cout << "COMPILING " << context << " ==> "  << m_curr_token->value << std::endl;
}

void Compiler::debug(const SymbolTable &st)
{
        std::string scopes [5] = {"unknown", "static", "field", "var", "arg" };
        std::cout << "SYMBOL TABLE CLASS SCOPE: " << st.get_class_name() << std::endl;
        for (const auto &s : st.view_class_scope()) {
                std::cout << "\t" << s.second.name << " | " << s.second.type << " | " << scopes[(int)s.second.scope] << " | " << s.second.index << std::endl;
        }
        std::cout << "SYMBOL TABLE SUBROUTINE SCOPE: " << st.get_subroutine_name() << std::endl;
        for (const auto &s : st.view_subroutine_scope()) {
                std::cout << "\t" << s.second.name << " | " << s.second.type << " | " << scopes[(int)s.second.scope] << " | " << s.second.index << std::endl;
        }
}

std::unique_ptr<AstNode> Compiler::compile_class()
{
        // 'class' className '{' classVarDec* subroutineDec* '}'
        std::unique_ptr<AstNode> compiled_class = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::CLASS });

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

        // classVarDec*
        if (lookahead_value() == "static" || lookahead_value() == "field") {
                do {
                        advance();
                        compiled_class->children.push_back(compile_class_var_dec());
                } while(lookahead_value() == "static" || lookahead_value() == "field");
        }

        //debug(m_st);

        // subroutineDec*
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
        std::unique_ptr<AstNode> class_var_dec = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::CLASS_VAR_DEC });

        // 'static' | 'field'
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
        std::unique_ptr<AstNode> subroutine_dec = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::SUBROUTINE_DEC });

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
        std::unique_ptr<AstNode> parameter_list = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::PARAMETER_LIST });

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
        std::unique_ptr<AstNode> subroutine_body = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::SUBROUTINE_BODY });

        // '{'
        assert(current_value() == "{");
        subroutine_body->children.push_back(make_node());

        // varDec*
        if (lookahead_value() == "var") {
                do {
                        advance();
                        subroutine_body->children.push_back(compile_var_dec());
                } while (lookahead_value() == "var");
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
        std::unique_ptr<AstNode> var_dec = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::VAR_DEC });

        // 'var'
        assert(current_value() == "var");
        var_dec->children.push_back(make_node());
        advance();

        // type
        var_dec->children.push_back(make_node());
        advance();

        // varName
        var_dec->children.push_back(make_node());

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
        std::unique_ptr<AstNode> statements = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::STATEMENTS });

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
        std::unique_ptr<AstNode> let_statement = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::LET_STATEMENT });

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
        std::unique_ptr<AstNode> if_statement = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::IF_STATEMENT });

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
        std::unique_ptr<AstNode> while_statement = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::WHILE_STATEMENT });

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
        std::unique_ptr<AstNode> do_statement = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::DO_STATEMENT });

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
        std::unique_ptr<AstNode> return_statement = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::RETURN_STATEMENT });

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
        std::unique_ptr<AstNode> expression = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::EXPRESSION });

        // term
        expression->children.push_back(compile_term());

        // (op term)*
        if (lookahead_value() == "+" || lookahead_value() == "-" || lookahead_value() == "*" ||
            lookahead_value() == "/" || lookahead_value() == "&" || lookahead_value() == "|" ||
            lookahead_value() == "<" || lookahead_value() == ">" || lookahead_value() == "=") {
                do {
                        advance();
                        // op
                        expression->children.push_back(make_node());
                        advance();

                        // term
                        expression->children.push_back(compile_term());
                } while (lookahead_value() == "+" || lookahead_value() == "-" || lookahead_value() == "*" ||
                         lookahead_value() == "/" || lookahead_value() == "&" || lookahead_value() == "|" ||
                         lookahead_value() == "<" || lookahead_value() == ">" || lookahead_value() == "=");
        }

        return expression;
}

std::unique_ptr<AstNode> Compiler::compile_term()
{
        // intConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' |
        // subroutineCall | '(' expression ')' | unaryOp term
        std::unique_ptr<AstNode> term = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::TERM });

        // keywordConstant
        if (current_value() == "true" || current_value() == "false" ||
            current_value() == "null" || current_value() == "this") {
                term->children.push_back(make_node());
                return term;
        }

        // stringConstant | intConstant
        if (current_type() == TOKEN_TYPE::INTEGER_CONST || current_type() == TOKEN_TYPE::STRING_CONST) {
                term->children.push_back(make_node());
                return term;
        }

        // unaryOp term
        if (current_value() == "-" || current_value() == "~") {
                // unaryOp
                term->children.push_back(make_node());
                advance();
                // term
                term->children.push_back(compile_term());

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
        if (lookahead_value() == "(" || lookahead_value() == ".") {
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
        std::unique_ptr<AstNode> subroutine_call = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::SUBROUTINE_CALL });

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
        std::unique_ptr<AstNode> expression_list = std::make_unique<AstNode>(AstNode { AST_NODE_TYPE::EXPRESSION_LIST });

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


std::vector<std::string> Compiler::generate_vm_code(const std::unique_ptr<AstNode> &ast)
{
        traverse_class(ast);

        return m_vm_code;
}

void Compiler::traverse_class(const std::unique_ptr<AstNode> &root)
{
        assert(root->ast_type == AST_NODE_TYPE::CLASS);

        for (std::unique_ptr<AstNode> &node : root->children) {
                switch (node->ast_type) {
                case AST_NODE_TYPE::TERMINAL_ELEMENT:
                        if (node->token_type == TOKEN_TYPE::IDENTIFIER) {
                                m_st.begin_class(node->terminal_value);
                        }
                        break;
                case AST_NODE_TYPE::CLASS_VAR_DEC:
                        traverse_class_var_dec(node);
                        debug(m_st);
                        break;

                case AST_NODE_TYPE::SUBROUTINE_DEC:
                        traverse_subroutine_dec(node);
                        debug(m_st);
                        break;
                default:
                        break;

                };

        }


}

void Compiler::traverse_class_var_dec(const std::unique_ptr<AstNode> &node)
{
        Symbol class_var;

        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();

        // scope
        std::string scope = (*it)->terminal_value;
        if (scope == "field") {
                class_var.scope = SCOPE::FIELD;
        }

        if (scope == "static") {
                class_var.scope = SCOPE::STATIC;
        }

        ++it;

        // type
        class_var.type = (*it)->terminal_value;

        ++it;

        // name
        class_var.name = (*it)->terminal_value;
        m_st.define_symbol(class_var);


        while ((*(it+1))->terminal_value == ",") {
                // ,
                ++it;

                // name
                ++it;
                class_var.name = (*it)->terminal_value;
                m_st.define_symbol(class_var);
        }
}

void Compiler::traverse_subroutine_dec(const std::unique_ptr<AstNode> &node)
{
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();
        // constructor | function | method
        std::string subroutine_type = (*it)->terminal_value;
        ++it;
        ++it;
        // subroutine name
        m_st.begin_subroutine((*it)->terminal_value);
        if (subroutine_type == "method") {
                // methods operate on k + 1 args
                m_st.define_symbol("this", m_st.get_class_name(), SCOPE::ARG);
        }

        while (it != node->children.cend()) {
                if ((*it)->ast_type == AST_NODE_TYPE::PARAMETER_LIST) {
                        traverse_parameter_list(*it);
                        ++it;
                        continue;
                }

                if ((*it)->ast_type == AST_NODE_TYPE::SUBROUTINE_BODY) {
                        traverse_subroutine_body(*it);
                        ++it;
                        continue;
                }

                ++it;
        }
}

void Compiler::traverse_parameter_list(const std::unique_ptr<AstNode> &node)
{
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();
        Symbol param;
        param.scope = SCOPE::ARG;
        param.type = (*it)->terminal_value;

        // name
        ++it;
        param.name = (*it)->terminal_value;
        m_st.define_symbol(param);

        while ((it+1) != node->children.cend() && (*(it+1))->terminal_value == ",") {
                // ,
                ++it;

                //type
                ++it;
                param.type = (*it)->terminal_value;

                // name
                ++it;
                param.name = (*it)->terminal_value;
                m_st.define_symbol(param);
        }
}


void Compiler::traverse_subroutine_body(const std::unique_ptr<AstNode> &node)
{
        for (const std::unique_ptr<AstNode> &node : node->children) {
                switch (node->ast_type) {
                case AST_NODE_TYPE::VAR_DEC :
                        traverse_var_dec(node);
                        break;
                case AST_NODE_TYPE::STATEMENTS :
                        // emit function after couting the nb of local vars
                        m_vme.emit_function(m_st.get_subroutine_name(), m_st.count_kind(SCOPE::VAR), m_vm_code);
                        traverse_statements(node);
                        break;
                default:
                        break;
                }
        }
}

void Compiler::traverse_var_dec(const std::unique_ptr<AstNode> &node)
{
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();
        Symbol var;
        var.scope = SCOPE::VAR;

        // type
        ++it;
        var.type = (*it)->terminal_value;

        // varname
        ++it;
        var.name = (*it)->terminal_value;
        m_st.define_symbol(var);

        while ((it+1) != node->children.cend() && (*(it+1))->terminal_value == ",") {
                // ,
                ++it;

                // varname
                ++it;
                var.name = (*it)->terminal_value;
                m_st.define_symbol(var);
        }
}


void Compiler::traverse_statements(const std::unique_ptr<AstNode> &node)
{
        for (const std::unique_ptr<AstNode> &node : node->children) {
                switch (node->ast_type) {
                case AST_NODE_TYPE::LET_STATEMENT:
                        traverse_let(node);
                        break;
                case AST_NODE_TYPE::IF_STATEMENT:
                case AST_NODE_TYPE::DO_STATEMENT:
                case AST_NODE_TYPE::WHILE_STATEMENT:
                case AST_NODE_TYPE::RETURN_STATEMENT:
                        traverse_return(node);
                        break;
                default:
                        break;
                }
        }
}

void Compiler::traverse_let(const std::unique_ptr<AstNode> &node)
{
        // grab pointer to variable name
        std::unique_ptr<AstNode> *let_identifier;
        std::vector<std::unique_ptr<AstNode>>::iterator it = node->children.begin();
        if ((*it)->token_type == TOKEN_TYPE::IDENTIFIER) {
                let_identifier = it.base();
        }

        // check if variable is an array
        ++it;

        if ((*it)->terminal_value == "[") {
                // eval expression
                ++it;
                traverse_expression(*it);
        }

        // skip until we get to '=' symbol to eval the right side
        while ((*it)->terminal_value != "=" && (*it)->token_type != TOKEN_TYPE::SYMBOL) {
                ++it;
        }

        ++it; // right side

        if ((*it)->ast_type == AST_NODE_TYPE::EXPRESSION) {
                traverse_expression(*it);
        }


        std::string identifier_name = (*let_identifier)->terminal_value;
        Symbol s;
        if (m_st.try_get(identifier_name, &s)) {
                m_vme.emit_pop(s.scope, s.index, m_vm_code);
        }
}

void Compiler::traverse_return(const std::unique_ptr<AstNode> &return_statement)
{
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = return_statement->children.cbegin();

        ++it; // return keyword

        if ((*it)->terminal_value == ";") {
                m_vme.emit_push(SEGMENT::CONSTANT, 0, m_vm_code);
                m_vme.emit_return(m_vm_code);
                return;
        }

        if ((*it)->ast_type == AST_NODE_TYPE::EXPRESSION) {
                traverse_expression((*it));
                return;
        }
}

void Compiler::traverse_expression(const std::unique_ptr<AstNode> &node)
{
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();

        // term
        traverse_term(*it);
        ++it;

        // (op term)*
        while (it != node->children.cend()) {
                        const std::unique_ptr<AstNode> *op = &(*it);

                        ++it; // term
                        traverse_term(*it);

                        if ((*op)->terminal_value == "*") m_vme.emit_call("Math.multiply", 2, m_vm_code);
                        if ((*op)->terminal_value == "/") m_vme.emit_call("Math.divide", 2, m_vm_code);
                        if ((*op)->terminal_value == "+") m_vme.emit_arithmetic(COMMAND::ADD, m_vm_code);
                        if ((*op)->terminal_value == "-") m_vme.emit_arithmetic(COMMAND::SUB, m_vm_code);
                        if ((*op)->terminal_value == "&") m_vme.emit_arithmetic(COMMAND::AND, m_vm_code);
                        if ((*op)->terminal_value == "|") m_vme.emit_arithmetic(COMMAND::OR, m_vm_code);
                        if ((*op)->terminal_value == ">") m_vme.emit_arithmetic(COMMAND::GT, m_vm_code);
                        if ((*op)->terminal_value == "<") m_vme.emit_arithmetic(COMMAND::LT, m_vm_code);
                        if ((*op)->terminal_value == "=") m_vme.emit_arithmetic(COMMAND::EQ, m_vm_code);
                        ++it;
        }
}

void Compiler::traverse_term(const std::unique_ptr<AstNode> &node)
{
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();
        while (it != node->children.cend()) {
                TOKEN_TYPE type = (*it)->token_type;
                AST_NODE_TYPE ast_type = (*it)->ast_type;
                std::string value = (*it)->terminal_value;

                if (type == TOKEN_TYPE::INTEGER_CONST) {
                        m_vme.emit_push(SEGMENT::CONSTANT, std::stoi(value), m_vm_code);
                        ++it;
                        continue;
                }

                if (type == TOKEN_TYPE::STRING_CONST) {
                        std::string str_val = (*it)->terminal_value;
                        size_t str_len = str_val.size();

                        // push str length on stack
                        m_vme.emit_push(SEGMENT::CONSTANT, str_len, m_vm_code);

                        // call String.new()
                        m_vme.emit_call("String.new", 1, m_vm_code);

                        // get ascii for each char in string
                        for (size_t i = 0; i < str_len; ++i) {
                                char c = str_val[i];
                                int ascii = (int)c;
                                m_vme.emit_push(SEGMENT::CONSTANT, ascii, m_vm_code);
                                // call String.appendChar on the new string instance
                                m_vme.emit_call("String.appendChar", 2, m_vm_code);
                        }

                        ++it;
                        continue;
                }

                if (type == TOKEN_TYPE::KEYWORD) {
                        if (value == "null" || value == "false") {
                                m_vme.emit_push(SEGMENT::CONSTANT, 0, m_vm_code);
                        }

                        if (value == "this") {
                                m_vme.emit_push(SEGMENT::POINTER, 0, m_vm_code);
                        }

                        if (value == "true") {
                                // -1
                                m_vme.emit_push(SEGMENT::CONSTANT, 1, m_vm_code);
                                m_vme.emit_arithmetic(COMMAND::NEG, m_vm_code);
                        }

                        ++it;
                        continue;
                }

                if (type == TOKEN_TYPE::SYMBOL) {
                        if (value == "~") {
                                // eval term first
                                traverse_term(*(it+1));
                                // emit NOT symbol
                                m_vme.emit_arithmetic(COMMAND::NOT, m_vm_code);
                                // jump over term
                                it += 2;
                                continue;
                        }

                        if (value == "-") {
                                // eval term first
                                traverse_term(*(it+1));

                                // emit NEG symbol
                                m_vme.emit_arithmetic(COMMAND::NEG, m_vm_code);

                                // jump over term
                                it += 2;
                                continue;
                        }

                        if (value == "(") {
                                ++it; // expression
                                traverse_expression(*it);
                                ++it;
                                continue;
                        }
                }

                if (type == TOKEN_TYPE::IDENTIFIER) {
                        std::vector<std::unique_ptr<AstNode>>::const_iterator lookahead_node = (it+1);

                        // className|varName.subroutineName ( expressionList )
                        if (lookahead_node != node->children.cend() &&
                            (*lookahead_node)->token_type == TOKEN_TYPE::SYMBOL &&
                            (*lookahead_node)->terminal_value == ".") {
                                std::string class_or_var_name = (*it)->terminal_value;
                                SCOPE scope = m_st.kind_of(class_or_var_name);
                                // check if method belongs to a var
                                if (scope == SCOPE::VAR) {
                                        m_vme.emit_push(SEGMENT::LOCAL, m_st.index_of(class_or_var_name), m_vm_code);
                                        class_or_var_name = m_st.type_of(class_or_var_name);
                                }

                                ++it; // .
                                ++it; // subroutineName
                                std::string subroutine_name = (*it)->terminal_value;
                                ++it; // (
                                ++it; // expressionList

                                int nb_args = traverse_expression_list(*it);

                                // if its a method, add an arg to the method. the reference of the object on which the method is supposed to operate on
                                if (scope == SCOPE::VAR) {
                                        ++nb_args;
                                }
                                m_vme.emit_call(class_or_var_name + "." + subroutine_name, nb_args, m_vm_code);
                                ++it;
                                continue;
                        }

                        // subrtouineName ( expressionList )
                        if (lookahead_node != node->children.cend() &&
                            (*lookahead_node)->token_type == TOKEN_TYPE::SYMBOL &&
                            (*lookahead_node)->terminal_value == "(") {
                                std::string subroutine_name = (*it)->terminal_value;
                                m_vme.emit_push(SEGMENT::POINTER, 0, m_vm_code);

                                ++it; // (
                                ++it; // expressionList

                                int nb_args = traverse_expression_list(*it);
                                m_vme.emit_call(m_st.get_class_name() + "." + subroutine_name, nb_args, m_vm_code);
                                ++it;
                                continue;
                        }


                        // varName [ expression ]
                        if (lookahead_node != node->children.cend() &&
                            (*lookahead_node)->token_type == TOKEN_TYPE::SYMBOL &&
                            (*lookahead_node)->terminal_value == "[") {
                                // get symbol
                                std::string var_name = (*it)->terminal_value;
                                Symbol s;
                                m_st.try_get(var_name, &s);
                                m_vme.emit_push(SEGMENT::LOCAL, s.index, m_vm_code);

                                ++it; // [
                                ++it; // expression

                                traverse_expression(*it);
                                m_vme.emit_arithmetic(COMMAND::ADD, m_vm_code);
                                ++it;
                                continue;

                        }

                        // local varName
                        Symbol s;
                        if (m_st.try_get(value, &s)) {
                                m_vme.emit_push(s.scope, s.index, m_vm_code);
                        }

                }

                // subrtouineName ( expressionList )
                if (ast_type == AST_NODE_TYPE::SUBROUTINE_CALL) {
                        traverse_term(*it);
                        ++it;
                        continue;
                }

                ++it;
        }
}

int Compiler::traverse_expression_list(const std::unique_ptr<AstNode> &node)
{
        int nb_args = 0;
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();
        if (it == node->children.cend()) return nb_args;

        traverse_expression(*it);
        ++nb_args;

        while ((it+1) != node->children.cend() && (*(it+1))->terminal_value == ",") {
                ++it; // ,
                traverse_expression(*it);
                ++nb_args;
        }

        return nb_args;
}
