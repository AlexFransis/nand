#include "translator.h"
#include "parser.h"
#include <ostream>
#include <stdexcept>
#include <vector>
#include <cassert>

namespace fs = std::filesystem;

bool VMTranslator::is_valid(const fs::path &file) {
        std::string valid_ext = ".vm";
        return file.extension() == valid_ext;
}

std::vector<fs::path> VMTranslator::get_valid_files(const fs::path &dir) {
        std::vector<fs::path> ret;
        for (const fs::directory_entry &e :
                     fs::recursive_directory_iterator(dir)) {
                fs::path file_p = e.path();
                if (is_valid(file_p)) {
                        ret.push_back(file_p);
                }
        }

        return ret;
}

VMTranslator::VMTranslator(const std::string &path, INPUT_TYPE input)
        :m_path(fs::path(path)),
         m_input_type(input)
{
        m_ifstream = std::ifstream();
        m_ofstream = std::ofstream();
}

VMTranslator::~VMTranslator()
{
        m_ifstream.close();
        m_ofstream.close();
}

void VMTranslator::translate()
{
        // TODO: remove IO from translator class
        // get file or files from dir
        if (m_input_type == INPUT_TYPE::D) {
                if (!fs::exists(m_path) || !fs::is_directory(m_path)) {
                        throw std::domain_error("Invalid directory path");
                }

                std::vector<fs::path> paths = get_valid_files(m_path);
                if (paths.size() == 0) {
                        throw std::domain_error("No valid files in directory");
                }

                std::copy(paths.begin(), paths.end(), std::back_inserter(m_files));
        }

        if (m_input_type == INPUT_TYPE::F) {
                if (!fs::exists(m_path) || !is_valid(m_path)) {
                        throw std::domain_error("Invalid file path");
                }

                m_files.push_back(m_path);
        }

        std::string out_ext = ".asm";
        if (m_input_type == INPUT_TYPE::F) {
                m_ofstream.open(m_path.replace_extension(out_ext));
        }

        if (m_input_type == INPUT_TYPE::D) {
                // create a file in the directory passed
                fs::path out = m_path / m_path.stem().replace_extension(out_ext);
                m_ofstream.open(out);
        }

        std::vector<fs::path>::const_iterator it = m_files.begin();
        while (it != m_files.end()) {
                m_ifstream.open(*it);
                m_ifstream.clear();
                m_ifstream.seekg(0);

                Parser parser (m_ifstream);

                while (parser.has_more_commands()) {
                        parser.advance();
                        Command c = parser.current_line();
                        std::string result = translate_command(c);

                        // m_ofstream << "Command type: " << command.command_type() << std::endl
                        //            << "Arg1: " << command.arg1() << std::endl
                        //            << "Arg2: " << command.arg2() << std::endl;
                }

                ++it;
        }
}

std::string VMTranslator::translate_command(const Command &c)
{
        if (c.command_type() == COMMAND_TYPE::C_PUSH || c.command_type() == COMMAND_TYPE::C_POP) {

        }

        if (c.command_type() == COMMAND_TYPE::C_ARITHMETIC) {
        }
}
