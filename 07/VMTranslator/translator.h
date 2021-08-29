#ifndef GUARD_translator_h
#define GUARD_translator_h


#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "command.h"

enum class INPUT_TYPE {
        DIR,
        FILE
};

class VMTranslator {
private:
        std::vector<std::filesystem::path> 	m_files;
        std::filesystem::path 			m_path;
        std::ifstream 				m_ifstream;
        std::ofstream 				m_ofstream;
        INPUT_TYPE				m_input_type;

        std::vector<std::filesystem::path>	get_valid_files(const std::filesystem::path &dir);
        bool					is_valid(const std::filesystem::path &file);
        std::string				translate_command(const Command &c);

      public:
        VMTranslator(const std::string &path, INPUT_TYPE input);
        ~VMTranslator();

        void translate();
        void print() const;
};


#endif
