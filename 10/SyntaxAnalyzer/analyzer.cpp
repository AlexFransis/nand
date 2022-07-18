#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include "analyzer.h"
#include "tokenizer.h"


namespace fs = std::filesystem;

Analyzer::Analyzer(const std::string &path, const std::string &input)
        :m_input_path(fs::path(path))
{
        m_ifstream = std::ifstream();
        m_ofstream = std::ofstream();
        m_input_type = input == "-d" ? INPUT_TYPE::DIR : INPUT_TYPE::FILE;
}

Analyzer::~Analyzer()
{
        m_ifstream.close();
        m_ofstream.close();
}

void Analyzer::write_xml(const std::unique_ptr<AstNode> &ast, std::ofstream &ofstream)
{
        std::unordered_map<std::string, std::string> token_map {
                {"SYMBOL" , "symbol"},
                {"IDENTIFIER" , "identifier"},
                {"KEYWORD" , "keyword"},
                {"STRING_CONST" , "stringConstant"},
                {"INTEGER_CONST" , "integerConstant"},
        };

        std::unordered_map<std::string, std::string> xml_special_chars {
                {"<", "&lt;"},
                {">", "&gt;"},
                {"&", "&amp;"},
                {"\"", "&quot;"},
                {"\'", "&apos;"},
        };

        std::function<void(const AstNode&, int)> inner = [&token_map, &xml_special_chars, &ofstream, &inner] (const AstNode &ast, int depth) -> void
                {
                        auto search = token_map.find(ast.type);
                        std::string type = (search != token_map.end() ? search->second : ast.type);

                        if (ast.terminal_value != std::string()) {
                                for (int i = 0; i < depth * 2; ++i) {
                                        ofstream << ' ';
                                }
                                ofstream << "<" << type << "> ";
                                search = xml_special_chars.find(ast.terminal_value);
                                std::string value = (search != xml_special_chars.end() ? search->second : ast.terminal_value);
                                ofstream << value << " </" << type << ">\n";
                        }

                        if (!ast.children.empty()) {
                                for (int i = 0; i < depth * 2; ++i) {
                                        ofstream << ' ';
                                }
                                ofstream << "<" << type << "> ";
                                ofstream << std::endl;
                                for (auto const &node : ast.children) {
                                        inner(*node, depth + 1);
                                }
                                for (int i = 0; i < depth * 2; ++i) {
                                        ofstream << ' ';
                                }
                                ofstream << "</" << type << ">\n";
                        }
                };

        inner(*ast, 0);
}


void Analyzer::begin()
{
        FileHandler fh;
        m_io_paths = fh.get_io_paths(m_input_path, m_input_type);
        std::vector<io_paths>::const_iterator it = m_io_paths.begin();
        while (it != m_io_paths.end()) {
                fs::path jack_file = it->first;
                fs::path xml_file = it->second;

                std::cout << "[INFO] Compiling file: " << std::string(jack_file) << std::endl;
                m_ifstream.open(jack_file);
                if (!m_ifstream.good()) {
                        std::string err = "[ERR] Could not open file: " + std::string(jack_file);
                        throw std::domain_error(err);
                }

                std::vector<Token> tokens;
                Tokenizer t;
                int line_number = 0;
                while (!m_ifstream.eof()) {
                        line_number++;
                        std::string current_line;
                        std::getline(m_ifstream, current_line);

                        if (!t.try_tokenize(current_line, tokens)) {
                                std::string err = "[ERR] Invalid syntax on line " + std::to_string(line_number) +
                                        " in file: " + std::string(jack_file);
                                throw std::domain_error(err);
                        }
                }

                Compiler c(tokens);
                std::unique_ptr<AstNode> ast = c.compile();

                std::cout << "[INFO] Opening output file: " << std::string(xml_file) << std::endl;
                m_ofstream.open(xml_file);
                if (!m_ofstream.good()) {
                        std::string err = "[ERR] Could not open file: " + std::string(xml_file);
                        throw std::domain_error(err);
                }

                write_xml(std::move(ast), m_ofstream);

                m_ifstream.close();
                m_ofstream.close();

                it++;
        }
}
