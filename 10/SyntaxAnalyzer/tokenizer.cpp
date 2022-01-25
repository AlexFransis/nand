#include "tokenizer.h"
#include <stdexcept>
#include <string>
#include <unordered_set>

bool Tokenizer::is_keyword(const std::string &s)
{
        const std::unordered_set<std::string> valid_keywords = { "class", "constructor", "function", "method",
                "field", "static", "var", "int", "char", "boolean", "void", "true", "false", "null", "this", "let", "do",
                "if", "else", "while", "return"};

        return valid_keywords.count(s);
}

bool Tokenizer::is_symbol(const std::string &s)
{
        const std::unordered_set<std::string> valid_symbols = {"{", "}", "(", ")", "[", "]", ".", ",",
                "+", "-", "*", "/", "&", "|", "<", ">", "=", "~"};

        return valid_symbols.count(s);
}

bool Tokenizer::is_integer(const std::string &s)
{
        std::string::const_iterator it = s.begin();
        while (it != s.end()) {
                if (!std::isdigit(*it)) return false;
                ++it;
        }

        int int_constant = std::stoi(s);

        return int_constant > 0 && int_constant < 32767;
}

bool Tokenizer::is_string(const std::string &s)
{
        return false;
}

bool Tokenizer::is_identifier(const std::string &s)
{
        return false;
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

bool Tokenizer::try_tokenize(const std::string &s, std::vector<std::string> &out_tokens)
{
        return true;
}
