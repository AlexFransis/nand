#ifndef GUARD_tokenizer_h
#define GUARD_tokenizer_h

#include <unordered_map>
#include <vector>
#include <string>

typedef std::vector<std::string> tokens;
typedef std::unordered_map<std::string, tokens> grammar;

class Tokenizer {
private:
public:
        bool 			is_keyword(const std::string &token);
        bool 			is_symbol(const std::string &token);
        bool 			is_integer(const std::string &token);
        bool 			is_string(const std::string &token);
        bool 			is_identifier(const std::string &token);
        bool 			is_ws_or_comment(const std::string &line);
        std::string 		trim_comments(const std::string &line);
        std::string 		token_type(const std::string &token);
        bool			try_tokenize(const std::string &s, std::vector<std::string> &out_tokens);
};

#endif
