#ifndef GUARD_tokenizer_h
#define GUARD_tokenizer_h

#include <unordered_map>
#include <vector>
#include <string>


struct Token {
        std::string type;
        std::string value;
};

class Tokenizer {
private:
        std::string 		ltrim(const std::string &s);
        std::string 		rtrim(const std::string &s);
        std::string 		trim_comments(const std::string &line);
        std::string 		trim_ws(const std::string &line);
        bool 			is_keyword(const std::string &token);
        bool 			is_symbol(const std::string &token);
        bool 			is_symbol(const char &c);
        bool 			is_integer(const std::string &token);
        bool 			is_string(const std::string &token);
        bool 			is_identifier(const std::string &token);
public:
        std::string 		get_token_type(const std::string &token);
        bool			try_tokenize(const std::string &line, std::vector<Token> &out_tokens);
};

#endif
