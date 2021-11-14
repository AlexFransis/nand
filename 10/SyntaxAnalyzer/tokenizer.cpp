#include "tokenizer.h"
#include <stdexcept>

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

std::string Tokenizer::trim_comments(const std::string &line)
{
	std::size_t pos_begin = line.find("//");
        if (pos_begin != std::string::npos) {
                return line.substr(0, pos_begin);
        }

        pos_begin = line.find("/*");
        if (pos_begin != std::string::npos) {
                std::size_t pos_end = line.find("*/");
                if (pos_end == std::string::npos) {
                }

        }

	return (pos_begin == std::string::npos) ? line : line.substr(0, pos_begin);
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
        return true;
}
