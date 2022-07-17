#ifndef GUARD_compiler_h
#define GUARD_compiler_h

#include <string>
#include <utility>
#include <vector>
#include "tokenizer.h"

struct AstNode {
        std::string type;
        std::string terminal_value;
        std::vector<std::unique_ptr<AstNode>> non_terminals;
};


class Compiler {
private:
        bool try_compile_class(std::vector<Token>::const_iterator &it, AstNode &ast_out);
public:
        bool try_compile(const std::vector<Token> &tokens, AstNode &ast_out);
};

#endif
