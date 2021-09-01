#ifndef GUARD_translator_h
#define GUARD_translator_h


#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "vm_command.h"

enum class INPUT_TYPE {
        DIR,
        FILE
};

class VMTranslator {
private:
        // IO stuff
        std::vector<std::filesystem::path> 	m_files;
        std::filesystem::path 			m_path;
        std::ifstream 				m_ifstream;
        std::ofstream 				m_ofstream;
        INPUT_TYPE				m_input_type;

        // ASM Rule


        std::vector<std::filesystem::path>	traverse_dir(const std::filesystem::path &dir, const std::string &ext);
        std::vector<std::filesystem::path>	get_valid_files(INPUT_TYPE input, const std::string &ext);
        void					open_output(INPUT_TYPE input, const std::string &ext);
        std::string				translate_command(const VMCommand &c);

      public:
        VMTranslator(const std::string &path, INPUT_TYPE input);
        ~VMTranslator();

        void begin();
        void print() const;
};


#endif
