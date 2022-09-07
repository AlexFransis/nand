#ifndef GUARD_ast_node_h
#define GUARD_ast_node_h

#include <string>
#include <vector>
#include <memory>
#include "tokenizer.h"

enum class AST_NODE_TYPE {
        TERMINAL_ELEMENT,
        CLASS,
        CLASS_VAR_DEC,
        SUBROUTINE_DEC,
        SUBROUTINE_BODY,
        SUBROUTINE_CALL,
        PARAMETER_LIST,
        VAR_DEC,
        STATEMENTS,
        DO_STATEMENT,
        LET_STATEMENT,
        WHILE_STATEMENT,
        RETURN_STATEMENT,
        IF_STATEMENT,
        EXPRESSION,
        EXPRESSION_LIST,
        TERM,
};

struct AstNode {
        AST_NODE_TYPE ast_type;
        TOKEN_TYPE token_type;
        std::string terminal_value;
        std::vector<std::unique_ptr<AstNode>> children;
};

#endif
