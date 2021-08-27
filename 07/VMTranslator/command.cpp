#include "command.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <memory>
#include <stdexcept>
#include <vector>

bool space(char c) { return isspace(c); }
bool not_space(char c) { return !isspace(c); }

COMMAND_TYPE get_command_type(const std::string &s)
{
        if (s == "add") 	{return C_ARITHMETIC;}
        if (s == "sub") 	{return C_ARITHMETIC;}
        if (s == "neg") 	{return C_ARITHMETIC;}
        if (s == "and") 	{return C_ARITHMETIC;}
        if (s == "not") 	{return C_ARITHMETIC;}
        if (s == "eq") 		{return C_ARITHMETIC;}
        if (s == "gt") 		{return C_ARITHMETIC;}
        if (s == "lt") 		{return C_ARITHMETIC;}
        if (s == "or") 		{return C_ARITHMETIC;}
        if (s == "push") 	{return C_PUSH;}
        if (s == "pop") 	{return C_POP;}
        if (s == "label") 	{return C_LABEL;}
        if (s == "goto") 	{return C_GOTO;}
        if (s == "goto") 	{return C_GOTO;}
        if (s == "if-goto") 	{return C_IF;}
        if (s == "function") 	{return C_FUNCTION;}
        if (s == "call") 	{return C_CALL;}

        return INVALID;
}

void Command::parse_str(const std::string &in_str, COMMAND_TYPE &out_type, std::string &out_arg1, std::string &out_arg2)
{
        typedef std::string::const_iterator str_iter;
        typedef std::vector<std::string>::const_iterator vec_iter;
        std::vector<std::string> ret;
        str_iter i = in_str.begin();
        str_iter j;

        while (i != in_str.end()) {
                // ignore leading spaces
                while (space(*i)) {
                        i++;
                }

                j = i;

                while (not_space(*j) && j != in_str.end()) {
                        j++;
                }

                // space found copy str [i , j)
                if (i != in_str.end()) {
                        ret.push_back(std::string(i, j));
                }

                i = j;
        }

        if (ret.size() < 0 || ret.size() > 3) {
                std::domain_error("invalid command: " + in_str);
        }

        vec_iter it = ret.begin();
        std::string s1 = *it++;
        out_type = get_command_type(s1);

        if (out_type == INVALID) {
                std::domain_error("invalid command type: " + s1);
        }

        if (it != ret.end()) {
                out_arg1 = *it++;
        }

        if (it != ret.end()) {
                out_arg2 = *it;
        }
}

Command::Command()
        :m_type(NOT_INIT),
         m_arg1(std::string()),
         m_arg2(std::string())
{
}

Command::Command(std::string line)
        :m_command(line),
         m_type(NOT_INIT),
         m_arg1(std::string()),
         m_arg2(std::string())

{
        parse_str(m_command, m_type, m_arg1, m_arg2);
}
