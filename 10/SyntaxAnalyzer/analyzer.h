#ifndef GUARD_analyzer_h
#define GUARD_analyzer_h

#include <fstream>
#include <memory>
#include "ast_node.h"
#include "tokenizer.h"

class Analyzer {
public:
        Analyzer();

        std::unique_ptr<AstNode> analyze(std::ifstream &input_stream);
};

#endif
