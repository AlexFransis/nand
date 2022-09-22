#ifndef GUARD_analyzer_h
#define GUARD_analyzer_h

#include <fstream>
#include <memory>
#include "ast_node.h"
#include "tokenizer.h"

class Analyzer {
public:
        std::unique_ptr<AstNode> compile(std::ifstream &input_stream);
        std::vector<std::string> compile_vm_code(std::ifstream &input_stream);
};

#endif
