#include <iostream>
#include "file_handler.h"
#include "analyzer.h"
#include "tokenizer.h"
#include "parser.h"


namespace fs = std::filesystem;

Analyzer::Analyzer(const std::string &path, const std::string &input)
{
        FileHandler fh (path, input);
        m_io_paths = fh.get_io_paths();
        m_ifstream = std::ifstream();
        m_ofstream = std::ofstream();
}

Analyzer::~Analyzer()
{
        m_ifstream.close();
        m_ofstream.close();
}

void Analyzer::begin()
{
        std::vector<io_paths>::const_iterator it = m_io_paths.begin();
        while (it != m_io_paths.end()) {
                fs::path jack_file = it->first;
                fs::path xml_file = it->second;

                m_ifstream.open(jack_file);
                m_ofstream.open(xml_file);

                if (!m_ifstream.good()) {
                        std::string err = "[ERR] Could not open file: " + std::string(jack_file);
                        throw std::domain_error(err);
                }
                std::cout << "[INFO] Opening input file: " << std::string(jack_file) << std::endl;

                if (!m_ofstream.good()) {
                        std::string err = "[ERR] Could not open file: " + std::string(xml_file);
                        throw std::domain_error(err);
                }
                std::cout << "[INFO] Opening output file: " << std::string(xml_file) << std::endl;

                int line_number = 0;
                while (!m_ifstream.eof()) {
                        line_number++;
                        std::string current_line;
                        std::getline(m_ifstream, current_line);

                        Tokenizer t;
                        if (t.is_ws_or_comment(current_line)) {
                                continue;
                        }

                        std::vector<std::pair<std::string, std::string>> tokens;
                        if (!t.try_tokenize(current_line, tokens)) {
                                std::string err = "[ERR] Invalid syntax on line " + std::to_string(line_number) +
                                        " in file: " + std::string(jack_file);
                                throw std::domain_error(err);
                        }

                        for (const std::pair<std::string, std::string> &token : tokens) {
                                m_ofstream << token.first << " ----> " << token.second << std::endl;
                        }
                }

        }
}
