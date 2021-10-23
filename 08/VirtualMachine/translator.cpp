#include <iostream>
#include <list>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdio>
#include "instr_mapper.h"
#include "parser.h"
#include "translator.h"

namespace fs = std::filesystem;

Translator::Translator(const std::string &path, INPUT_TYPE input)
        :m_input_path(fs::path(path)),
         m_input_type(input)
{
        m_ifstream = std::ifstream();
        m_ofstream = std::ofstream();
}

Translator::~Translator()
{
        m_ifstream.close();
        m_ofstream.close();
}

void Translator::begin()
{
        std::string out_ext = ".asm";
        std::string in_ext = ".vm";

        m_files = get_valid_files(m_input_type, in_ext);
        open_output(m_input_type, out_ext);

        std::vector<fs::path>::const_iterator it = m_files.begin();
        while (it != m_files.end()) {
                m_ifstream.clear();
                m_ifstream.close();
                m_ifstream.open(*it);

                if (!m_ifstream.good()) {
                        std::string err = "[ERR] Could not open file: " + std::string(*it);
                        throw std::domain_error(err);
                }

                std::cout << "[INFO] Begin translation: " << std::string(*it) << std::endl;
                int line_number = 0;
                Parser p;
                InstructionMapper mapper (it->stem());
                while (!m_ifstream.eof()) {
                        line_number++;
                        Command vmc;
                        std::string current_line;
                        std::getline(m_ifstream, current_line);

                        if (!p.try_parse(current_line, vmc))
                        {
                                std::string err = "[ERR] Invalid command on line " + std::to_string(line_number) +
                                        " in file: " + std::string(it->stem());
                                throw std::domain_error(err);
                        }

                        std::list<std::string> asm_instrs = mapper.map_command(vmc);
                        for (const std::string &instr : asm_instrs) {
                                m_ofstream << instr << std::endl;
                        }
                }

                ++it;
        }
}

std::vector<fs::path> Translator::get_valid_files(INPUT_TYPE input, const std::string &ext)
{
        std::vector<fs::path> result;
        if (m_input_type == INPUT_TYPE::DIR) {
                if (!fs::exists(m_input_path) || !fs::is_directory(m_input_path)) {
                        throw std::domain_error("[ERR] Invalid directory path");
                }

                std::vector<fs::path> files = traverse_dir(m_input_path, ext);
                if (files.size() == 0) {
                        throw std::domain_error("[ERR] No valid files in directory");
                }

                std::copy(files.begin(), files.end(), std::back_inserter(result));
        }

        if (m_input_type == INPUT_TYPE::FILE) {
                if (!fs::exists(m_input_path) || m_input_path.extension() != ext) {
                        throw std::domain_error("[ERR] Invalid file path");
                }

                result.push_back(m_input_path);
        }

        return result;
}

void Translator::open_output(INPUT_TYPE input, const std::string &ext)
{
        if (m_input_type == INPUT_TYPE::FILE) {
                fs::path out = m_input_path.replace_extension(ext);
                m_ofstream.open(out);
                if (!m_ofstream.good()) {
                        throw std::domain_error("[ERR] Could not open output file");
                }
                std::cout << "[INFO] Opening output file: " << std::string(out) << std::endl;
        }

        if (m_input_type == INPUT_TYPE::DIR) {
                // create a file in the directory passed
                fs::path out = m_input_path / m_input_path.stem().replace_extension(ext);
                m_ofstream.open(out);
                if (!m_ofstream.good()) {
                        throw std::domain_error("[ERR] Could not open output file");
                }
                std::cout << "[INFO] Opening output file: " << std::string(out) << std::endl;
        }
}

std::vector<fs::path> Translator::traverse_dir(const fs::path &dir, const std::string &ext) {
        std::vector<fs::path> result;
        for (const fs::directory_entry &e : fs::recursive_directory_iterator(dir)) {
                fs::path file_p = e.path();
                if (file_p.extension() == ext) {
                        result.push_back(file_p);
                }
        }

        return result;
}
