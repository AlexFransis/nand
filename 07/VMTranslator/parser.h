#ifndef GUARD_parser_h
#define GUARD_parser_h

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include "command.h"

class Parser {
private:
        std::ifstream				&m_ifstream;
        Command					m_current_line;

        bool					is_valid_command(const std::string &line);
        std::string				trim_comments(const std::string &line);
        std::string				trim_ws(const std::string &line);

public:
        Parser(std::ifstream &ifstream);

        inline Command 				current_line() const {return m_current_line;};
        bool					has_more_commands() const;
        void					advance();
};

#endif
