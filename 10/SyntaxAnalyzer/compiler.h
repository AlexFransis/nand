#ifndef GUARD_compiler_h
#define GUARD_compiler_h

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "tokenizer.h"

struct AstNode {
        std::string type;
        std::string terminal_value;
        std::vector<std::unique_ptr<AstNode>> children;
};


class Compiler {
private:
        std::vector<Token>::const_iterator m_curr_token;
        void advance();

        std::unique_ptr<AstNode> compile_class();
        std::unique_ptr<AstNode> compile_class_var_dec();
        std::unique_ptr<AstNode> compile_subroutine_dec();
        std::unique_ptr<AstNode> compile_parameter_list();

public:
        Compiler(const std::vector<Token> &tokens);
        std::unique_ptr<AstNode> compile();
};

#endif
