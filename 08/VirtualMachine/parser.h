#ifndef GUARD_parser_h
#define GUARD_parser_h

#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "command.h"

// <key: command_name, val: <first: command_type, second: args>>
typedef std::unordered_map<std::string, std::pair<std::string, unsigned int>> command_rules;

class Parser {
private:
        std::ifstream				&m_ifstream;
        std::string				m_current_line;
        unsigned int				m_line_number;
        static command_rules 			m_command_rules;

        bool					is_command_valid(const std::string &line);
        std::string				trim_comments(const std::string &line);
        std::string				trim_ws(const std::string &line);
        std::vector<std::string>		tokenize(const std::string &s);

public:
        Parser(std::ifstream &ifstream);
        Parser();

        inline std::string 			current_line() {return m_current_line;};
        bool					is_command_valid();
        bool					try_advance();
        Command					parse_current();
        Command					parse(const std::string &s);
};

#endif
