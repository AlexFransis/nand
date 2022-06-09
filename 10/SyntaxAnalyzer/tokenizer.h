#ifndef GUARD_tokenizer_h
#define GUARD_tokenizer_h

#include <unordered_map>
#include <vector>
#include <string>

typedef std::vector<std::pair<std::string, std::string>> tokens;

class Tokenizer {
private:
        bool 			is_keyword(const std::string &token);
        bool 			is_symbol(const std::string &token);
        bool 			is_symbol(const char &c);
        bool 			is_integer(const std::string &token);
        bool 			is_string(const std::string &token);
        bool 			is_identifier(const std::string &token);
public:
        std::string 		get_token_type(const std::string &token);
        bool			try_tokenize(const std::string &line, tokens &out_tokens);
};

#endif
