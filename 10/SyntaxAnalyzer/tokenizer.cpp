#include "tokenizer.h"

const grammar Tokenizer::grammar = {
        {"<keyword>", 		{"class constructor function method field static var int char\
 				boolean void true false null this let do if else while return"}},
        {"<symbol>",		{"{ } ( ) [ ] . , ; + - * / & | < > = ~"}},
        {"<integer>",		{"0..32767"}},
        {"<string>",		{"\"", "UNICODE", "\""}},
        {"<identifier>",	{}}
};

std::vector<std::string> split(const std::string &s, char delimiter) {
        std::vector<std::string> result;
        std::string::const_iterator i = s.begin();
        std::string::const_iterator j;

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
                        result.push_back(std::string(i, j));
                }

                i = j;
        }

        return result;
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

std::string trim_comments(const std::string &line)
{
	std::size_t pos = line.find("//");
	return (pos == std::string::npos) ? line : line.substr(0, pos);
}

std::string trim_ws(const std::string &line)
{
	return rtrim(ltrim(line));
}

bool Tokenizer::is_ws_or_comment(const std::string &line)
{
        if (trim_comments(line) == std::string())
                return true;
        if (trim_ws(line) == std::string())
                return true;
        return false;
}

bool Tokenizer::try_tokenize(const std::string &s, std::vector<std::pair<std::string, std::string>> &tokens)
{
        std::vector<std::string> splits = split(s, ' ');
}
