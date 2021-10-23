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

struct Command {
        std::string 			type;
        std::string 			name;
        std::vector<std::string>	args;

        Command()
        {
        }
        Command(const std::string &type, const std::string &name, const std::vector<std::string> &args)
                : type(type),
                  name(name),
                  args(args)
        {
        }

        inline std::string arg1() const { return args.size() > 0 ? args[0] : std::string();}
        inline std::string arg2() const { return args.size() > 1 ? args[1] : std::string();}
};

class Translator {
private:
        // IO stuff
        std::vector<std::filesystem::path> 	m_files;
        std::filesystem::path 			m_input_path;
        std::ifstream 				m_ifstream;
        std::ofstream 				m_ofstream;
        INPUT_TYPE				m_input_type;

        std::vector<std::filesystem::path>	traverse_dir(const std::filesystem::path &dir, const std::string &ext);
        std::vector<std::filesystem::path>	get_valid_files(INPUT_TYPE input, const std::string &ext);
        void					open_output(INPUT_TYPE input, const std::string &ext);

      public:
        Translator(const std::string &path, INPUT_TYPE input);
        ~Translator();

        void begin();
};

#endif
