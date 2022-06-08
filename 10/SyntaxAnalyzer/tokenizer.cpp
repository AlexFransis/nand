#include "tokenizer.h"
#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>

bool Tokenizer::is_keyword(const std::string &s)
{
        return s == "class" ||
                s == "constructor" ||
                s == "function" ||
                s == "method" ||
                s == "field" ||
                s == "static" ||
                s == "var" ||
                s == "int" ||
                s == "char" ||
                s == "boolean" ||
                s == "void" ||
                s == "true" ||
                s == "false" ||
                s == "null" ||
                s == "this" ||
                s == "let" ||
                s == "do" ||
                s == "if" ||
                s == "else" ||
                s == "while" ||
                s == "return";
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

bool Tokenizer::is_integer(const std::string &s)
{
        std::string::const_iterator it = s.begin();
        while (it != s.end()) {
                if (!std::isdigit(*it)) return false;
                ++it;
        }

        int int_constant = std::stoi(s);

        return int_constant >= 0 && int_constant < 32768; // 2^15
}

bool Tokenizer::is_string(const std::string &s)
{
        std::string::const_iterator first = s.begin();
        if (*first != '"') return false;

        std::string::const_iterator end = s.end();
        if (*(--end) != '"') return false;

        // remove leading and ending double quotes
        std::string stripped = s.substr(1, s.size() - 2);

        first = stripped.begin();
        end = stripped.end();

        int double_quote_unicode = 0x22;
        int line_feed_unicode = 0x0a;
        while (first != end) {
                int char_unicode = (int)*first;
                if (char_unicode == double_quote_unicode || char_unicode == line_feed_unicode) return false;
                first++;
        }


        return true;
}

bool Tokenizer::is_identifier(const std::string &s)
{
        if (is_keyword(s)) return false;
        std::string::const_iterator first = s.begin();
        if (std::isdigit(*first)) return false;

        int digit_0_unicode = 0x30;
        int digit_9_unicode = 0x39;
        int alpha_A_unicode = 0x41;
        int alpha_Z_unicode = 0x5a;
        int alpha_a_unicode = 0x61;
        int alpha_z_unicode = 0x7a;
        int underscore_unicode = 0x5f;

        while (first != s.end()) {
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

std::string Tokenizer::get_token_type(const std::string &token)
{
        if (is_keyword(token))
                return "keyword";
        if (is_symbol(token))
                return "symbol";
        if (is_integer(token))
                return "integer_const";
        if (is_string(token))
                return "string_const";
        if (is_identifier(token))
                return "identifier";
        return "unknown";
}

bool Tokenizer::try_tokenize(const std::string &s, std::vector<std::pair<std::string, std::string>> &out_tokens)
{
        std::string::const_iterator i = s.begin();
        std::string::const_iterator j;

        while (i != s.end()) {
                // ignore leading spaces
                while (isspace(*i)) {
                        i++;
                }

                j = i;

                // string token begining
                if (*j == '"') {
                        j++;
                        while (*j != '"' && j != s.end()) {
                                j++;
                        }

                        if (j == s.end()) {
                                return false;
                        }

                        j++; // grab closing double quote

                        // string found
                        std::string token (i, j);
                        std::string type = get_token_type(token);
                        if (type == "unknown") {
                                return false;
                        }

                        out_tokens.push_back(std::make_pair(token, type));

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
                std::string type = get_token_type(token);
                if (type == "unknown") {
                        return false;
                }

                out_tokens.push_back(std::make_pair(token, type));

                i = j;
        }

        return true;
}
