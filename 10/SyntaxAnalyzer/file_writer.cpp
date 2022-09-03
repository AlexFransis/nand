#include <iostream>
#include <unordered_map>
#include <functional>
#include "file_writer.h"

namespace fs = std::filesystem;


fs::path FileWriter::construct_output_file(const std::string &ext)
{
        fs::path out_file = m_jack_file;
        out_file.replace_extension(ext);
        return out_file;
}

void FileWriter::write_xml(const std::unique_ptr<AstNode> &ast)
{

        fs::path xml_file = construct_output_file(xml_ext);
        std::ofstream ofstream;

        std::cout << "[INFO] Writing XML file: " << std::string(xml_file) << std::endl;
        ofstream.open(xml_file);
        if (!ofstream.good()) {
                std::string err = "[ERR] Could not write file: " + std::string(xml_file);
                throw std::domain_error(err);
        }

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

        std::function<void(const AstNode&, int)> recur = [&token_map, &xml_special_chars, &ofstream, &recur] (const AstNode &ast, int depth) -> void
                {
                        auto search = token_map.find(ast.type);
                        std::string type = (search != token_map.end() ? search->second : ast.type);

                        if (ast.terminal_value != std::string()) {
                                for (int i = 0; i < depth * 2; ++i) {
                                        ofstream << " ";
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
                                ofstream << "<" << type << ">";
                                ofstream << std::endl;
                                for (auto const &node : ast.children) {
                                        recur(*node, depth + 1);
                                }
                                for (int i = 0; i < depth * 2; ++i) {
                                        ofstream << ' ';
                                }
                                ofstream << "</" << type << ">\n";
                        }
                };

        recur(*ast, 0);
}


void FileWriter::write_vm_commands(const std::vector<std::string> &vmc)
{
        fs::path vm_file = construct_output_file(vm_ext);
        std::ofstream ofstream;

        std::cout << "[INFO] Writing VM file: " << std::string(vm_file) << std::endl;
        ofstream.open(vm_file);
        if (!ofstream.good()) {
                std::string err = "[ERR] Could not write file: " + std::string(vm_file);
                throw std::domain_error(err);
        }

        for (const std::string &s : vmc) {
                ofstream << s << std::endl;
        }
}
