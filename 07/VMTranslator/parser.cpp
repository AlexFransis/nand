#include "parser.h"

Parser::Parser(std::ifstream &ifstream)
        : m_ifstream(ifstream)
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
        } while (!is_valid_command(line));

        std::string no_comments = trim_comments(line);
        std::string no_ws = trim_ws(no_comments);
        Command command (no_ws);
        m_current_line = command;
}
