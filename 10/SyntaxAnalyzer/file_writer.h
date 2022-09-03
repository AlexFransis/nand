#ifndef GUARD_file_writer_h
#define GUARD_file_writer_h

#include <memory>
#include <fstream>
#include <filesystem>
#include "ast_node.h"


class FileWriter {
private:
        std::string vm_ext = ".vm";
        std::string xml_ext = ".xml";
        std::filesystem::path m_jack_file;
        std::filesystem::path construct_output_file(const std::string &ext);
public:
        FileWriter(const std::filesystem::path &jack_file)
                : m_jack_file(jack_file) {};
        void write_xml(const std::unique_ptr<AstNode> &ast);
        void write_vm_commands(const std::vector<std::string> &vmc);
};

#endif
