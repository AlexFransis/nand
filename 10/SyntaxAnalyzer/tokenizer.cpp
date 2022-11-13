#include "tokenizer.h"
#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>


std::string Tokenizer::ltrim(const std::string &s)
{
	const std::string ws = " \n\r\t\f\v";
	size_t start = s.find_first_not_of(ws);
	return (start == std::string::npos) ? std::string() : s.substr(start);
}

std::string Tokenizer::rtrim(const std::string &s)
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

std::string Tokenizer::trim_ws(const std::string &line)
{
	return rtrim(ltrim(line));
}

bool Tokenizer::is_keyword(const std::string &token)
{
        return token == "class" ||
                token == "constructor" ||
                token == "function" ||
                token == "method" ||
                token == "field" ||
                token == "static" ||
                token == "var" ||
                token == "int" ||
                token == "char" ||
                token == "boolean" ||
                token == "void" ||
                token == "true" ||
                token == "false" ||
                token == "null" ||
                token == "this" ||
                token == "let" ||
                token == "do" ||
                token == "if" ||
                token == "else" ||
                token == "while" ||
                token == "return";
}

bool Tokenizer::is_symbol(const std::string &token)
{
        return token == "{" ||
                token == "}" ||
                token == "(" ||
                token == ")" ||
                token == "[" ||
                token == "]" ||
                token == "." ||
                token == "," ||
                token == ";" ||
                token == "+" ||
                token == "-" ||
                token == "*" ||
                token == "/" ||
                token == "&" ||
                token == "|" ||
                token == "<" ||
                token == ">" ||
                token == "=" ||
                token == "~";
}

bool Tokenizer::is_symbol(const char &c)
{
        std::string s (1, c);
        return is_symbol(s);
}

bool Tokenizer::is_integer(const std::string &token)
{
        std::string::const_iterator it = token.begin();
        while (it != token.end()) {
                if (!std::isdigit(*it)) return false;
                ++it;
        }

        int num = std::stoi(token);

        return num >= 0x0000 && num < 0x8000; // 2^15
}

bool Tokenizer::is_string(const std::string &token)
{
        if (token.front() != '"' && token.back() != '"') return false;

        // remove leading and ending double quotes
        std::string stripped = token.substr(1, token.size() - 2);
        std::string::const_iterator first = stripped.begin();

        int double_quote_unicode = 0x22;
        int line_feed_unicode = 0x0a;
        while (first != stripped.end()) {
                int char_unicode = (int)*first;
                if (char_unicode == double_quote_unicode || char_unicode == line_feed_unicode) return false;
                first++;
        }

        return true;
}

bool Tokenizer::is_identifier(const std::string &token)
{
        std::string::const_iterator first = token.begin();
        if (std::isdigit(*first)) return false;

        int digit_0_unicode = 0x30;
        int digit_9_unicode = 0x39;
        int alpha_A_unicode = 0x41;
        int alpha_Z_unicode = 0x5a;
        int alpha_a_unicode = 0x61;
        int alpha_z_unicode = 0x7a;
        int underscore_unicode = 0x5f;

        while (first != token.end()) {
                int char_unicode = (int)*first;
                if (!(char_unicode >= digit_0_unicode && char_unicode <= digit_9_unicode) &&
                    !(char_unicode >= alpha_A_unicode && char_unicode <= alpha_Z_unicode) &&
                    !(char_unicode >= alpha_a_unicode && char_unicode <= alpha_z_unicode) &&
                    !(char_unicode == underscore_unicode)) {
                        return false;
                }

                first++;
        }

        return true;
}

TOKEN_TYPE Tokenizer::get_token_type(const std::string &token)
{
        if (is_keyword(token)) return TOKEN_TYPE::KEYWORD;
        if (is_symbol(token)) return TOKEN_TYPE::SYMBOL;
        if (is_integer(token)) return TOKEN_TYPE::INTEGER_CONST;
        if (is_string(token)) return TOKEN_TYPE::STRING_CONST;
        if (is_identifier(token)) return TOKEN_TYPE::IDENTIFIER;

        return TOKEN_TYPE::UNKNOWN;
}

bool Tokenizer::try_tokenize(const std::string &line, std::vector<Token> &out_tokens)
{
        std::string trimmed = trim_ws(trim_comments(line));
        std::string::const_iterator i = trimmed.begin();
        std::string::const_iterator j;

        while (i != trimmed.end()) {
                // ignore leading spaces
                while (isspace(*i)) {
                        i++;
                }

                j = i;

                // string token begining
                if (*j == '"') {
                        j++;
                        while (*j != '"' && j != trimmed.end()) {
                                j++;
                        }

                        if (j == trimmed.end()) {
                                return false;
                        }

                        j++; // grab closing double quote

                        // string found
                        std::string token (i, j);
                        TOKEN_TYPE type = get_token_type(token);
                        if (type == TOKEN_TYPE::UNKNOWN) {
                                return false;
                        }

                        std::string stripped = token.substr(1, token.size() - 2);
                        Token t { type, stripped };
                        out_tokens.push_back(t);

                        i = j;
                        continue;
                }

                while (!isspace(*j) && !is_symbol(*j)) {
                        j++;
                }

                // check if single character token
                if (i == j) {
                        j++;
                }
                // token found
                std::string token (i, j);
                TOKEN_TYPE type = get_token_type(token);
                if (type == TOKEN_TYPE::UNKNOWN) {
                        return false;
                }

                Token t { type, token };
                out_tokens.push_back(t);

                i = j;
        }

        return true;
}
