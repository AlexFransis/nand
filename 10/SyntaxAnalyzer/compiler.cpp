#include "compiler.h"
#include <memory>


bool Compiler::try_compile(const std::vector<Token> &tokens, AstNode &ast_out)
{
        std::vector<Token>::const_iterator it = tokens.begin();
        if (!try_compile_class(it, ast_out)) {
                return false;
        }
        return true;
}


bool Compiler::try_compile_class(std::vector<Token>::const_iterator &it, AstNode &ast_out)
{
        // 'class' className '{' classVarDec* subroutineDec* '}'
        if (it->value != "class") return false;
        ast_out.type = "class";
        ++it;
        if (it->type != "IDENTIFIER") return false;
        AstNode identifier;
        identifier.type = it->type;
        identifier.terminal_value = it->value;
        ast_out.non_terminals.push_back(std::make_unique<AstNode>(identifier));

}
