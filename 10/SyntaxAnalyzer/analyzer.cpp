#include "analyzer.h"
#include "tokenizer.h"
#include "compiler.h"


Analyzer::Analyzer()
{
        m_vm_commands = std::vector<std::string>();
}

std::unique_ptr<AstNode> Analyzer::analyze(std::ifstream &input_stream)
{
        std::vector<Token> tokens;
        Tokenizer t;
        int line_number = 0;
        while (!input_stream.eof()) {
                line_number++;
                std::string current_line;
                std::getline(input_stream, current_line);

                if (!t.try_tokenize(current_line, tokens)) {
                        std::string err = "[ERR] Invalid syntax on line " + std::to_string(line_number);
                        throw std::domain_error(err);
                }
        }

        Compiler c(tokens);
        std::unique_ptr<AstNode> ast = c.compile();
        m_vm_commands = c.get_vm_commands();

        return ast;
}
