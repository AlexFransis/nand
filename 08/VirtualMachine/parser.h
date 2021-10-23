#ifndef GUARD_parser_h
#define GUARD_parser_h

#include <string>
#include <unordered_map>
#include <vector>

// <key: command_name, val: <first: command_type, second: args>>
typedef std::unordered_map<std::string, std::pair<std::string, unsigned int>> command_rules;

struct Command {
        std::string 			type;
        std::string 			name;
        std::vector<std::string>	args;

        Command() {}
        Command(const std::string &type, const std::string &name, const std::vector<std::string> &args)
                : type(type), name(name), args(args) {}

        inline std::string arg1() const { return args.size() > 0 ? args[0] : std::string();}
        inline std::string arg2() const { return args.size() > 1 ? args[1] : std::string();}
};

class Parser {
private:
        static command_rules 			m_command_rules;
        bool					is_command_valid(const std::string &line);
        std::string				trim_comments(const std::string &line);
        std::string				trim_ws(const std::string &line);
        std::vector<std::string>		tokenize(const std::string &s);

public:
        Parser();
        bool					try_parse(const std::string &s, Command &out_command);
};

#endif
