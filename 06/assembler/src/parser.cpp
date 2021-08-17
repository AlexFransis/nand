#include <cassert>
#include "parser.h"


Parser::Parser(std::ifstream &fstream)
	:m_fstream(fstream)
{
}

Parser::~Parser()
{
	m_fstream.close();
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

std::string Parser::trim_ws(const std::string &s)
{
	return rtrim(ltrim(s));
}

std::string Parser::trim_comments(const std::string &s)
{
	std::size_t pos = s.find("//");
	return (pos == std::string::npos) ? s : s.substr(0, pos);
}

bool Parser::has_more_commands() const
{
        int c = m_fstream.peek();
	return !(c == EOF);
}

bool Parser::is_valid_command(const std::string &s)
{
        if (is_ws(s))
                return false;
        if (is_comment(s))
                return false;
        if (command_type(s) == INVALID)
                return false;

        return true;
}

bool Parser::is_comment(const std::string &s)
{
        std::string no_ws = trim_ws(s);
        std::string delim = "//";
        size_t pos = no_ws.find_first_of(delim);

        if (pos == 0)
                return true;

        return false;
}

bool Parser::is_ws(const std::string &s)
{
        std::string no_ws = trim_ws(s);
        if (no_ws.empty())
                return true;

        return false;
}

void Parser::advance()
{
	std::string line;
        do {
                std::getline(m_fstream, line);
        } while (!is_valid_command(line));

	std::string no_comments = trim_comments(line);
	std::string no_ws = trim_ws(no_comments);
	m_line = no_ws;
}

std::string Parser::current_line() const
{
	return m_line;
}

Command Parser::command_type(const std::string &curr_line)
{
	const char& first_char = curr_line.front();

	if (first_char == '@') {
		return A_COMMAND;
	}

	if (first_char == '(') {
                const std::string delim = ")";
                size_t pos = curr_line.find_first_of(delim);
                if (pos == std::string::npos)
                        return INVALID;
		return L_COMMAND;
	}

	const std::string delim = ";=";
        size_t pos = curr_line.find_first_of(delim);
        if (pos != std::string::npos) {
                return C_COMMAND;
        }

        return INVALID;
}

std::string Parser::symbol(const std::string &curr_line)
{
	assert(command_type(curr_line) == A_COMMAND || command_type(curr_line) == L_COMMAND);
	const std::string delim = "@(";
	size_t pos = curr_line.find_first_of(delim);
	return (pos == std::string::npos) ? std::string() : curr_line.substr(pos + 1);
}

std::string Parser::dest(const std::string &curr_line)
{
	assert(command_type(curr_line) == C_COMMAND);
	const std::string delim = "=";
	size_t pos = curr_line.find_first_of(delim);
	return (pos == std::string::npos) ? std::string() : curr_line.substr(0, pos);
}

std::string Parser::comp(const std::string &curr_line)
{
	assert(command_type(curr_line) == C_COMMAND);
	const std::string delim1 = "=";
	const std::string delim2 = ";";
	size_t pos1 = curr_line.find_first_of(delim1);
	size_t pos2 = curr_line.find_first_of(delim2);

	// D=A+1
	if (pos1 != std::string::npos && pos2 == std::string::npos) {
		return curr_line.substr(pos1 + 1);
	}

	// A+1;JMP
	if (pos1 == std::string::npos && pos2 != std::string::npos) {
		return curr_line.substr(0, pos2);
	}

	// A+1
	if (pos1 == std::string::npos && pos2 == std::string::npos) {
		return curr_line;
	}

	return std::string();
}

std::string Parser::jump(const std::string &curr_line)
{
	assert(command_type(curr_line) == C_COMMAND);
	const std::string delim = ";";
	size_t pos = curr_line.find_first_of(delim);
	return pos == std::string::npos ? std::string() : curr_line.substr(pos + 1);
}
