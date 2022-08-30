#include "vm_emitter.h"
#include <cassert>

std::vector<std::string> VMEmitter::emit_vm_commands(const std::unique_ptr<AstNode> &node)
{
        std::vector<std::string> vm_commands;
        SymbolTable st;
        // traverse ast root node and keep track of symbols
        traverse_ast_root_node(node, st, vm_commands);
        traverse_ast_child_nodes(node, st, vm_commands);
        return vm_commands;
}

void VMEmitter::traverse_ast_root_node(const std::unique_ptr<AstNode> &root, SymbolTable &st, std::vector<std::string> &out_vm_commands)
{
        assert(root->type == "class");
        for (std::unique_ptr<AstNode> &child : root->children) {
                if (child->type == "identifier") {
                        st.begin_class(child->terminal_value);
                }
        }

}

void VMEmitter::traverse_ast_child_nodes(const std::unique_ptr<AstNode> &root, SymbolTable &st, std::vector<std::string> &out_vm_commands)
{
}
