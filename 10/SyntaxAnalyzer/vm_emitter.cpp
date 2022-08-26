#include "vm_emitter.h"

std::vector<std::string> VMEmitter::emit_vm_commands(const AstNode &node)
{
        std::vector<std::string> vm_commands;
        SymbolTable st;
        // traverse ast and keep track of symbols
        traverse_ast(node, st, vm_commands);
        return vm_commands;
}

void VMEmitter::traverse_ast(const AstNode &root, SymbolTable &st, std::vector<std::string> &out_vm_commands)
{

}
