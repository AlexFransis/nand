#ifndef GUARD_parser_h
#define GUARD_parser_h

#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "command.h"

class Parser {
private:
        std::ifstream						&m_ifstream;
        std::string						m_current_line;
        unsigned int						m_line_number;
        // < key: command, val: < first: command_type, second: nb args > >
        // < push : < C_PUSH : 2 > >
        std::unordered_map<std::string,
                std::pair<std::string, unsigned int>> 		m_command_rules;

        bool					is_valid_command(const std::string &line);
        std::string				trim_comments(const std::string &line);
        std::string				trim_ws(const std::string &line);
        std::vector<std::string>		tokenize(const std::string &s);

public:
        Parser(std::ifstream &ifstream);

        inline std::string 			current_line() const {return m_current_line;};
        bool					has_more_commands() const;
        void					advance();
        Command					parse();
        Command					parse(const std::string &s);
};

#endif
