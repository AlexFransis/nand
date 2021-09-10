#include "parser.h"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <string>
#include <cassert>
#include <unordered_map>

Parser::Parser(std::ifstream &ifstream)
        : m_ifstream(ifstream),
          m_command_rules({
                        // command : < command_type , nb_args >
                        {"add", 	std::make_pair("C_ARITHMETIC", 0)},
                        {"sub", 	std::make_pair("C_ARITHMETIC", 0)},
                        {"neg", 	std::make_pair("C_ARITHMETIC", 0)},
                        {"eq", 		std::make_pair("C_ARITHMETIC", 0)},
                        {"gt", 		std::make_pair("C_ARITHMETIC", 0)},
                        {"lt", 		std::make_pair("C_ARITHMETIC", 0)},
                        {"and", 	std::make_pair("C_ARITHMETIC", 0)},
                        {"or", 		std::make_pair("C_ARITHMETIC", 0)},
                        {"not", 	std::make_pair("C_ARITHMETIC", 0)},

                        {"push", 	std::make_pair("C_PUSH", 2)},
                        {"pop", 	std::make_pair("C_POP", 2)},
                        {"label", 	std::make_pair("C_LABEL", 1)},
                        {"goto", 	std::make_pair("C_GOTO", 1)},
                        {"if-goto", 	std::make_pair("C_IF", 1)},
                        {"function", 	std::make_pair("C_FUNCTION", 2)},
                        {"call", 	std::make_pair("C_CALL", 2)},
                        {"return", 	std::make_pair("C_RETURN", 0)},
                })
{
}

std::string ltrim(const std::string &s)
{
	const std::string ws = " \n\r\t\f\v";
	size_t start = s.find_first_not_of(ws);
	return (start == std::string::npos) ? std::string() : s.substr(start);
}

std::string rtrim(const std::string &s)
{
	const std::string ws = " \n\r\t\f\v";
	size_t end = s.find_last_not_of(ws);
	return (end == std::string::npos) ? std::string() : s.substr(0, end + 1);
}

std::string Parser::trim_comments(const std::string &line)
{
	std::size_t pos = line.find("//");
	return (pos == std::string::npos) ? line : line.substr(0, pos);
}

std::string Parser::trim_ws(const std::string &line)
{
	return rtrim(ltrim(line));
}

bool Parser::has_more_commands() const
{
	return m_ifstream.peek() != EOF;
}

bool Parser::is_valid_command(const std::string &line)
{
        if (trim_comments(line) == std::string())
                return false;
        if (trim_ws(line) == std::string())
                return false;
        return true;
}

void Parser::advance()
{
        std::string line;
        do {
                std::getline(m_ifstream, line);
                ++m_line_number;
        } while (!is_valid_command(line));

        std::string no_comments = trim_comments(line);
        std::string no_ws = trim_ws(no_comments);
        m_current_line = no_ws;
}

std::vector<std::string> split(const std::string &s, char delimiter) {
        std::vector<std::string> ret;
        typedef std::string::const_iterator str_iter;
        str_iter i = s.begin();
        str_iter j;

        while (i != s.end()) {
                // ignore leading spaces
                while (isspace(*i)) {
                        i++;
                }

                j = i;

                while (*j != delimiter && j != s.end()) {
                        j++;
                }

                // delim found copy str [i , j)
                if (i != s.end()) {
                        ret.push_back(std::string(i, j));
                }

                i = j;
        }

        return ret;
}


std::vector<std::string> Parser::tokenize(const std::string &s)
{
        std::vector<std::string> splits = split(s, ' ');
        // to lower only the command
        std::vector<std::string>::iterator it = splits.begin();
        std::transform(it->begin(), it->end(), it->begin(), ::tolower);

        return splits;
}

VMCommand Parser::parse() {
        return parse(m_current_line);
}


VMCommand Parser::parse(const std::string &s)
{
        typedef std::unordered_map<std::string, std::pair<std::string, unsigned int>> command_rules;
        typedef std::vector<std::string>::size_type vec_size;

        std::vector<std::string> tokens = tokenize(s);
        std::vector<std::string>::const_iterator it = tokens.begin();

        if (it == tokens.end()) {
                std::domain_error("[ERROR] Line: " + std::to_string(m_line_number) + ": invalid command");
        }

        std::string command = *it++;

        command_rules::const_iterator found = m_command_rules.find(command);
        if (found == m_command_rules.end()) {
                std::domain_error("[ERROR] Line: " + std::to_string(m_line_number) + ": invalid command type");
        }

        std::vector<std::string> args;
        while (it != tokens.end()) {
                args.push_back(*it);
                ++it;
        }

        std::string command_type = found->second.first;
        vec_size nb_args = found->second.second;
        if (args.size() != nb_args) {
                std::domain_error("[ERROR] Line: " + std::to_string(m_line_number) + ": invalid args");
        }

        return VMCommand (command_type, command, args);
}
