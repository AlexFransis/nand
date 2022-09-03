#ifndef GUARD_analyzer_h
#define GUARD_analyzer_h

#include <fstream>
#include <memory>
#include "ast_node.h"
#include "tokenizer.h"

class Analyzer {
private:
        std::vector<std::string> m_vm_commands;
public:
        Analyzer();

        std::unique_ptr<AstNode> analyze(std::ifstream &input_stream);
        inline std::vector<std::string> get_vm_commands() { return m_vm_commands; }
};

#endif
