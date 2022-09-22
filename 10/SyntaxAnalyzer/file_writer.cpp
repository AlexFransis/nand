#include <iostream>
#include <unordered_map>
#include <functional>
#include "file_writer.h"
#include "tokenizer.h"

namespace fs = std::filesystem;


fs::path FileWriter::construct_output_file(const std::string &ext)
{
        fs::path out_file = m_jack_file;
        out_file.replace_extension(ext);
        return out_file;
}

void FileWriter::write_xml(const std::unique_ptr<AstNode> &ast)
{
        int indent = 4;
        fs::path xml_file = construct_output_file(xml_ext);
        std::ofstream ofstream;

        std::cout << "[INFO] Writing XML file: " << std::string(xml_file) << std::endl;
        ofstream.open(xml_file);
        if (!ofstream.good()) {
                std::string err = "[ERR] Could not write file: " + std::string(xml_file);
                throw std::domain_error(err);
        }

        std::unordered_map<TOKEN_TYPE, std::string> token_map {
                {TOKEN_TYPE::SYMBOL , "SYMBOL"},
                {TOKEN_TYPE::IDENTIFIER , "IDENTIFIER"},
                {TOKEN_TYPE::KEYWORD , "KEYWORD"},
                {TOKEN_TYPE::STRING_CONST , "STRING_CONST"},
                {TOKEN_TYPE::INTEGER_CONST , "INTEGER_CONST"},
        };

        std::unordered_map<AST_NODE_TYPE, std::string> ast_type_map {
                { AST_NODE_TYPE::CLASS, "CLASS" },
                { AST_NODE_TYPE::CLASS_VAR_DEC, "CLASS_VAR_DEC" },
                { AST_NODE_TYPE::SUBROUTINE_DEC, "SUBROUTINE_DEC" },
                { AST_NODE_TYPE::SUBROUTINE_BODY, "SUBROUTINE_BODY" },
                { AST_NODE_TYPE::SUBROUTINE_CALL, "SUBROUTINE_CALL" },
                { AST_NODE_TYPE::PARAMETER_LIST, "PARAMETER_LIST" },
                { AST_NODE_TYPE::VAR_DEC, "VAR_DEC" },
                { AST_NODE_TYPE::STATEMENTS, "STATEMENTS" },
                { AST_NODE_TYPE::DO_STATEMENT, "DO_STATEMENT" },
                { AST_NODE_TYPE::LET_STATEMENT, "LET_STATEMENT" },
                { AST_NODE_TYPE::WHILE_STATEMENT, "WHILE_STATEMENT" },
                { AST_NODE_TYPE::RETURN_STATEMENT, "RETURN_STATEMENT" },
                { AST_NODE_TYPE::IF_STATEMENT, "IF_STATEMENT" },
                { AST_NODE_TYPE::EXPRESSION, "EXPRESSION" },
                { AST_NODE_TYPE::EXPRESSION_LIST, "EXPRESSION_LIST" },
                { AST_NODE_TYPE::TERM, "TERM" },
        };


        std::unordered_map<std::string, std::string> xml_special_chars {
                {"<", "&lt;"},
                {">", "&gt;"},
                {"&", "&amp;"},
                {"\"", "&quot;"},
                {"\'", "&apos;"},
        };

        std::function<void(const AstNode&, int)> recur = [&token_map, &ast_type_map, &xml_special_chars,
                                                          &ofstream, &recur, &indent] (const AstNode &ast, int depth) -> void
                {
                        if (ast.ast_type == AST_NODE_TYPE::TERMINAL_ELEMENT) {
                                auto token_type = token_map.find(ast.token_type);
                                for (int i = 0; i < depth * indent; ++i) {
                                        ofstream << " ";
                                }
                                ofstream << "<" << token_type->second << "> ";
                                auto xml_element = xml_special_chars.find(ast.terminal_value);
                                bool is_xml_special_char = xml_element != xml_special_chars.end();
                                std::string value = is_xml_special_char ? token_type->second : ast.terminal_value;
                                ofstream << value << " </" << token_type->second << ">\n";
                                return;
                        }

                        for (int i = 0; i < depth * indent; ++i) {
                                ofstream << ' ';
                        }
                        auto ast_type = ast_type_map.find(ast.ast_type);
                        ofstream << "<" << ast_type->second << ">";
                        ofstream << std::endl;
                        for (auto const &node : ast.children) {
                                recur(*node, depth + 1);
                        }
                        for (int i = 0; i < depth * indent; ++i) {
                                ofstream << ' ';
                        }
                        ofstream << "</" << ast_type->second << ">\n";

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
