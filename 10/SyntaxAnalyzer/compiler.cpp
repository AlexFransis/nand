#include "compiler.h"


bool Compiler::try_compile(const std::vector<Token> &tokens, AstNode &ast_out)
{
        std::vector<Token>::const_iterator it = tokens.begin();
        while (it != tokens.end()) {
                Token t = *it;
                if (t.value != "class") {
                        return false;
                }

                ast_out.type = "class";
        }
        return true;
}
