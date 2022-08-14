#ifndef GUARD_xml_writer_h
#define GUARD_xml_writer_h

#include <fstream>
#include "file_handler.h"
#include "tokenizer.h"
#include "compiler.h"

class Analyzer {
private:
        std::ifstream			m_ifstream;
        std::ofstream			m_ofstream;
        std::filesystem::path 		m_input_path;
        INPUT_TYPE			m_input_type;
        std::vector<io_paths>		m_io_paths;

        void 				write_xml(const std::unique_ptr<AstNode> &ast, std::ofstream &ofstream);

public:
        Analyzer(const std::string &path, const std::string &input);
        ~Analyzer();

        void				analyze();
};

#endif
