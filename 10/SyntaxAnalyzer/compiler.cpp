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
        /* 'class' className '{' classVarDec* subroutineDec* '}' */
        ast_out.type = "class";

        // 'class'
        if (it->type != "KEYWORD" && it->value != "class") return false;
        ast_out.children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
        ++it;

        // className
        if (it->type != "IDENTIFIER") return false;
        ast_out.children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
        ++it;

        // '{'
        if (it->type != "SYMBOL" && it->value != "{") return false;
        ast_out.children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
        ++it;

        // classVarDec*
        while (it->value == "static" || it->value == "field") {
                if (!try_compile_class_var_dec(it, ast_out)) return false;
                ++it;
        }

        // subroutineDec*
        while (it->value == "constructor" || it->value == "function" || it->value == "method") {
                if (!try_compile_subroutine(it, ast_out)) return false;
                ++it;
        }

        // '}'
        if (it->type != "SYMBOL" && it->value != "}") return false;
        ast_out.children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
        ++it;

        return true;
}

bool Compiler::try_compile_class_var_dec(std::vector<Token>::const_iterator &it, AstNode &ast_out)
{
        /* ('static' | 'field') type varName (',' varName)* ';' */
        std::unique_ptr<AstNode> class_var_dec = std::make_unique<AstNode>(AstNode { "classVarDec" });

        // 'static' | 'field'
        if (it->value != "static" && it->value != "field") return false;
        class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
        ++it;

        // type
        if (it->value != "int" && it->value != "char" && it->value != "boolean") return false;
        class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
        ++it;

        // varName
        if (it->type != "IDENTIFIER") return false;
        class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
        ++it;

        // (, varName)*
        while (it->value != ";") {
                if (it->value != ",") return false;
                class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
                ++it;
                class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));
                ++it;
        }

        // ';'
        class_var_dec->children.push_back(std::make_unique<AstNode>(AstNode { it->type, it->value }));

        ast_out.children.push_back(std::move(class_var_dec));

        return true;
}

bool Compiler::try_compile_subroutine(std::vector<Token>::const_iterator &it, AstNode &ast_out)
{
        return true;
}
