#ifndef GUARD_parser_h
#define GUARD_parser_h

#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "translator.h"

// <key: command_name, val: <first: command_type, second: args>>
typedef std::unordered_map<std::string, std::pair<std::string, unsigned int>> command_rules;

class Parser {
private:
        static command_rules 			m_command_rules;
        bool					is_command_valid(const std::string &line);
        std::string				trim_comments(const std::string &line);
        std::string				trim_ws(const std::string &line);
        std::vector<std::string>		tokenize(const std::string &s);

public:
        Parser();
        bool					try_parse(const std::string &s, Command &out_command);
};

#endif
