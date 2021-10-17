#include <iostream>
#include <list>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdio>
#include "command_translator.h"
#include "parser.h"
#include "translator.h"

namespace fs = std::filesystem;

FileHandler::FileHandler(const std::string &path, INPUT_TYPE input)
        :m_path(fs::path(path)),
         m_input_type(input)
{
        m_ifstream = std::ifstream();
        m_ofstream = std::ofstream();
}

FileHandler::~FileHandler()
{
        m_ifstream.close();
        m_ofstream.close();
}

void FileHandler::begin()
{
        std::string out_ext = ".asm";
        std::string in_ext = ".vm";

        m_files = get_valid_files(m_input_type, in_ext);
        open_output(m_input_type, out_ext);

        std::vector<fs::path>::const_iterator it = m_files.begin();
        while (it != m_files.end()) {
                std::cout << "[INFO] Begin translation: " << std::string(*it) << std::endl;
                m_ifstream.clear();
                m_ifstream.seekg(0);
                m_ifstream.open(*it);
                std::string filename = it->stem();

                Parser p (m_ifstream);
                CommandTranslator ct (filename);
                while (p.has_more_commands()) {
                        p.advance();
                        Command vmc = p.parse_current();
                        std::list<std::string> asm_instrs = ct.translate_command(vmc);

                        for (const std::string &instr : asm_instrs) {
                                m_ofstream << instr << std::endl;
                        }
                }

                ++it;
        }
}

std::vector<fs::path> FileHandler::get_valid_files(INPUT_TYPE input, const std::string &ext)
{
        std::vector<fs::path> result;
        if (m_input_type == INPUT_TYPE::DIR) {
                if (!fs::exists(m_path) || !fs::is_directory(m_path)) {
                        throw std::domain_error("[ERR] Invalid directory path");
                }

                std::vector<fs::path> files = traverse_dir(m_path, ext);
                if (files.size() == 0) {
                        throw std::domain_error("[ERR] No valid files in directory");
                }

                std::copy(files.begin(), files.end(), std::back_inserter(result));
        }

        if (m_input_type == INPUT_TYPE::FILE) {
                if (!fs::exists(m_path) || m_path.extension() != ext) {
                        throw std::domain_error("[ERR] Invalid file path");
                }

                result.push_back(m_path);
        }

        return result;
}

void FileHandler::open_output(INPUT_TYPE input, const std::string &ext)
{
        if (m_input_type == INPUT_TYPE::FILE) {
                fs::path out = m_path.replace_extension(ext);
                std::cout << "[INFO] Opening output file: " << std::string(out) << std::endl;
                m_ofstream.open(out);
        }

        if (m_input_type == INPUT_TYPE::DIR) {
                // create a file in the directory passed
                fs::path out = m_path / m_path.stem().replace_extension(ext);
                std::cout << "[INFO] Opening output file: " << std::string(out) << std::endl;
                m_ofstream.open(out);
        }
}

std::vector<fs::path> FileHandler::traverse_dir(const fs::path &dir, const std::string &ext) {
        std::vector<fs::path> result;
        for (const fs::directory_entry &e : fs::recursive_directory_iterator(dir)) {
                fs::path file_p = e.path();
                if (file_p.extension() == ext) {
                        result.push_back(file_p);
                }
        }

        return result;
}
