#include <unordered_map>
#include <functional>
#include "file_writer.h"

FileWriter::FileWriter(){};

void FileWriter::write_xml(const std::unique_ptr<AstNode> &ast, std::ofstream &ofstream)
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
