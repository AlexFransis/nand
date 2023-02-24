#include "compiler.h"
#include "ast_node.h"
#include "symbol_table.h"
#include "vm_emitter.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>


Compiler::Compiler()
{
        m_vme = VMEmitter();
        m_st = SymbolTable();
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

std::vector<std::string> Compiler::generate_vm_code(const std::unique_ptr<AstNode> &ast)
{
        compile_class(ast);

        return m_vm_code;
}

void Compiler::compile_class(const std::unique_ptr<AstNode> &root)
{
        for (std::unique_ptr<AstNode> &node : root->children) {
                switch (node->ast_type) {
                case AST_NODE_TYPE::TERMINAL_ELEMENT:
                        if (node->token_type == TOKEN_TYPE::IDENTIFIER) {
                                m_st.begin_class(node->terminal_value);
                        }
                        break;
                case AST_NODE_TYPE::CLASS_VAR_DEC:
                        compile_class_var_dec(node);
                        break;
                case AST_NODE_TYPE::SUBROUTINE_DEC:
                        compile_subroutine_dec(node);
                        break;
                default:
                        break;
                };
        }
}

void Compiler::compile_class_var_dec(const std::unique_ptr<AstNode> &node)
{
        Symbol class_var;
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();

        std::string scope = (*it)->terminal_value;
        if (scope == "field") {
                class_var.scope = SCOPE::FIELD;
        }

        if (scope == "static") {
                class_var.scope = SCOPE::STATIC;
        }

        ++it;
        class_var.type = (*it)->terminal_value;
        ++it;
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

void Compiler::compile_subroutine_dec(const std::unique_ptr<AstNode> &node)
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
                        compile_parameter_list(*it);
                        ++it;
                        continue;
                }

                if ((*it)->ast_type == AST_NODE_TYPE::SUBROUTINE_BODY) {
                        compile_subroutine_body(*it, subroutine_type);
                        ++it;
                        continue;
                }

                ++it;
        }
}

void Compiler::compile_parameter_list(const std::unique_ptr<AstNode> &node)
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


void Compiler::compile_subroutine_body(const std::unique_ptr<AstNode> &node, std::string subroutine_type)
{
        int if_label_idx = 0;
        int while_label_idx = 0;

        for (const std::unique_ptr<AstNode> &node : node->children) {
                switch (node->ast_type) {
                case AST_NODE_TYPE::VAR_DEC :
                        compile_var_dec(node);
                        break;
                case AST_NODE_TYPE::STATEMENTS :
                        // emit function after couting the nb of local vars
                        m_vme.emit_function(m_st.get_subroutine_name(), m_st.count_kind(SCOPE::VAR), m_vm_code);

                        // call mem alloc
                        if (subroutine_type == "constructor") {
                                int field_count = m_st.count_kind(SCOPE::FIELD);
                                m_vme.emit_push(SEGMENT::CONSTANT, field_count, m_vm_code);
                                m_vme.emit_call("Memory.alloc", 1, m_vm_code);
                                m_vme.emit_pop(SEGMENT::POINTER, 0, m_vm_code);
                        }

                        // set object reference to THIS
                        if (subroutine_type == "method") {
                                m_vme.emit_push(SEGMENT::ARGUMENT, 0, m_vm_code);
                                m_vme.emit_pop(SEGMENT::POINTER, 0, m_vm_code);
                        }

                        compile_statements(node, &if_label_idx, &while_label_idx);
                        break;
                default:
                        break;
                }
        }
}

void Compiler::compile_var_dec(const std::unique_ptr<AstNode> &node)
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


void Compiler::compile_statements(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx)
{
        for (const std::unique_ptr<AstNode> &node : node->children) {
                switch (node->ast_type) {
                case AST_NODE_TYPE::LET_STATEMENT:
                        compile_let(node);
                        break;
                case AST_NODE_TYPE::DO_STATEMENT:
                        // pop unused retrun value from void method
                        compile_term(node);
                        m_vme.emit_pop(SEGMENT::TEMP, 0, m_vm_code);
                        break;
                case AST_NODE_TYPE::IF_STATEMENT:
                        compile_if(node, if_label_idx, while_label_idx);
                        break;
                case AST_NODE_TYPE::WHILE_STATEMENT:
                        compile_while(node, if_label_idx, while_label_idx);
                        break;
                case AST_NODE_TYPE::RETURN_STATEMENT:
                        compile_return(node);
                        break;
                default:
                        break;
                }
        }
}

void Compiler::compile_let(const std::unique_ptr<AstNode> &node)
{
        std::vector<std::unique_ptr<AstNode>>::iterator it = node->children.begin();
        ++it; // skip let keyword

        // grab pointer to variable name
        std::vector<std::unique_ptr<AstNode>>::iterator let_identifier;
        let_identifier = it;


        // skip until we get to '=' symbol to eval the right side
        while ((*it)->terminal_value != "=") {
                ++it;
        }

        ++it; // right side

        // eval right side
        if ((*it)->ast_type == AST_NODE_TYPE::EXPRESSION) {
                compile_expression(*it);
        }

        // eval left side
        // lookahead to check if variable name is for an array
        if ((*(let_identifier+1))->token_type == TOKEN_TYPE::SYMBOL && (*(let_identifier+1))->terminal_value == "[") {
                std::string identifier_name = (*let_identifier)->terminal_value;

                // eval array expression
                ++let_identifier; // [
                ++let_identifier; // expression
                compile_expression(*let_identifier);

                // push array name
                Symbol s;
                if (m_st.try_get(identifier_name, &s)) {
                        m_vme.emit_push(s.scope, s.index, m_vm_code);
                }

                m_vme.emit_arithmetic(COMMAND::ADD, m_vm_code);

                // pop address to `that` segment
                m_vme.emit_pop(SEGMENT::POINTER, 1, m_vm_code);
                m_vme.emit_pop(SEGMENT::THAT, 0, m_vm_code);
        } else {
                std::string identifier_name = (*let_identifier)->terminal_value;
                Symbol s;
                if (m_st.try_get(identifier_name, &s)) {
                        m_vme.emit_pop(s.scope, s.index, m_vm_code);
                }
        }
}

void Compiler::compile_return(const std::unique_ptr<AstNode> &return_statement)
{
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = return_statement->children.cbegin();

        ++it; // return keyword

        if ((*it)->terminal_value == ";") {
                m_vme.emit_push(SEGMENT::CONSTANT, 0, m_vm_code);
                m_vme.emit_return(m_vm_code);
                return;
        }

        if ((*it)->ast_type == AST_NODE_TYPE::EXPRESSION) {
                compile_expression((*it));
                m_vme.emit_return(m_vm_code);
                return;
        }
}

void Compiler::compile_expression(const std::unique_ptr<AstNode> &node)
{
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();

        // term
        compile_term(*it);
        ++it;

        // (op term)*
        while (it != node->children.cend()) {
                        const std::unique_ptr<AstNode> *op = &(*it);

                        ++it; // term
                        compile_term(*it);

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

void Compiler::compile_term(const std::unique_ptr<AstNode> &node)
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
                                compile_term(*(it+1));
                                // emit NOT symbol
                                m_vme.emit_arithmetic(COMMAND::NOT, m_vm_code);
                                // jump over term
                                it += 2;
                                continue;
                        }

                        if (value == "-") {
                                // eval term first
                                compile_term(*(it+1));

                                // emit NEG symbol
                                m_vme.emit_arithmetic(COMMAND::NEG, m_vm_code);

                                // jump over term
                                it += 2;
                                continue;
                        }

                        if (value == "(") {
                                ++it; // expression
                                compile_expression(*it);
                                ++it;
                                continue;
                        }
                }

                if (type == TOKEN_TYPE::IDENTIFIER) {
                        std::vector<std::unique_ptr<AstNode>>::const_iterator lookahead_node = (it+1);

                        // className|varName.subroutineName ( expressionList )
                        if (lookahead_node != node->children.cend() && (*lookahead_node)->token_type == TOKEN_TYPE::SYMBOL && (*lookahead_node)->terminal_value == ".") {
                                std::string class_or_var_name = (*it)->terminal_value;
                                // check if method belongs to a var or field
                                SCOPE scope = m_st.kind_of(class_or_var_name);
                                int nb_args = 0;

                                // identifier is not a var/field/static
                                if (scope != SCOPE::UNKNOWN) {
                                        m_vme.emit_push(scope, m_st.index_of(class_or_var_name), m_vm_code);
                                        class_or_var_name = m_st.type_of(class_or_var_name);
                                        // add an argument to the method. the reference of object that is calling the method
                                        ++nb_args;
                                }

                                ++it; // .
                                ++it; // subroutineName
                                std::string subroutine_name = (*it)->terminal_value;
                                ++it; // (
                                ++it; // expressionList

                                nb_args = nb_args + compile_expression_list(*it);
                                m_vme.emit_call(class_or_var_name + "." + subroutine_name, nb_args, m_vm_code);
                                ++it;
                                continue;
                        }

                        // subroutineName ( expressionList )
                        if (lookahead_node != node->children.cend() &&
                            (*lookahead_node)->token_type == TOKEN_TYPE::SYMBOL &&
                            (*lookahead_node)->terminal_value == "(") {
                                std::string subroutine_name = (*it)->terminal_value;
                                m_vme.emit_push(SEGMENT::POINTER, 0, m_vm_code);

                                ++it; // (
                                ++it; // expressionList

                                int nb_args = compile_expression_list(*it) + 1;
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
                                m_vme.emit_push(s.scope, s.index, m_vm_code);

                                ++it; // [
                                ++it; // expression

                                compile_expression(*it);
                                m_vme.emit_arithmetic(COMMAND::ADD, m_vm_code);
                                m_vme.emit_pop(SEGMENT::POINTER, 1, m_vm_code);
                                m_vme.emit_push(SEGMENT::THAT, 0, m_vm_code);
                                ++it;
                                continue;

                        }

                        // local varName
                        Symbol s;
                        if (m_st.try_get(value, &s)) {
                                m_vme.emit_push(s.scope, s.index, m_vm_code);
                        }

                }

                // subroutineName ( expressionList )
                if (ast_type == AST_NODE_TYPE::SUBROUTINE_CALL) {
                        compile_term(*it);
                        ++it;
                        continue;
                }

                ++it;
        }
}

int Compiler::compile_expression_list(const std::unique_ptr<AstNode> &node)
{
        int nb_args = 0;
        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();
        if (it == node->children.cend()) return nb_args;

        compile_expression(*it);
        ++nb_args;

        ++it;

        while (it != node->children.cend() && (*it)->terminal_value == ",") {
                ++it;
                compile_expression(*it);
                ++nb_args;

                ++it; // ,
        }

        return nb_args;
}

void Compiler::compile_if(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx)
{
        std::string else_statement_begin = "ELSE_BEGIN" + std::to_string(*if_label_idx);
        std::string else_statement_end = "ELSE_END" + std::to_string(*if_label_idx);
        ++(*if_label_idx); // increment label idx

        bool has_else_statement = false;
        for (const std::unique_ptr<AstNode> &child : node->children) {
                if (child->token_type == TOKEN_TYPE::KEYWORD && child->terminal_value == "else") {
                        has_else_statement = true;
                        break;
                }
        }

        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();

        it++;  // if
        it++; // (

        // compute !(cond)
        compile_expression(*it);
        m_vme.emit_arithmetic(COMMAND::NOT, m_vm_code);

        // goto ELSE_BEGIN label
        m_vme.emit_if(else_statement_begin, m_vm_code);

        while ((*it)->ast_type != AST_NODE_TYPE::STATEMENTS) {
                ++it;
        }

        // evaluate IF statements
        compile_statements(*it, if_label_idx, while_label_idx);

        // goto ELSE_END label
        m_vme.emit_goto(else_statement_end, m_vm_code);

        // ELSE_BEGIN label
        m_vme.emit_label(else_statement_begin, m_vm_code);
        if (has_else_statement) {
                do { ++it; }
                while ((*it)->ast_type != AST_NODE_TYPE::STATEMENTS);

                // evaluate ELSE statements
                compile_statements(*it, if_label_idx, while_label_idx);
        }

        // ELSE_END label
        m_vme.emit_label(else_statement_end, m_vm_code);
}

void Compiler::compile_while(const std::unique_ptr<AstNode> &node, int *if_label_idx, int *while_label_idx)
{
        std::string while_begin = "WHILE_BEGIN" + std::to_string(*while_label_idx);
        std::string while_end = "WHILE_END" + std::to_string(*while_label_idx);
        ++(*while_label_idx); // increment label idx

        std::vector<std::unique_ptr<AstNode>>::const_iterator it = node->children.cbegin();

        it++;  // while
        it++; // (

        // WHILE_BEGIN label
        m_vme.emit_label(while_begin, m_vm_code);

        // compute !(cond)
        compile_expression(*it);
        m_vme.emit_arithmetic(COMMAND::NOT, m_vm_code);

        // goto WHILE_END label
        m_vme.emit_if(while_end, m_vm_code);

        while ((*it)->ast_type != AST_NODE_TYPE::STATEMENTS) {
                ++it;
        }

        // evaluate WHILE statements
        compile_statements(*it, if_label_idx, while_label_idx);

        // goto WHILE_BEGIN label
        m_vme.emit_goto(while_begin, m_vm_code);

        // WHILE_END label
        m_vme.emit_label(while_end, m_vm_code);
}
