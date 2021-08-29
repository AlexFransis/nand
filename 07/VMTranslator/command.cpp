#include "command.h"
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>

Command::Command(const std::string &type)
        : m_type(type),
          m_args(std::vector<std::string>())
{
}

Command::Command(const std::string &type, const std::vector<std::string> &args)
        : m_type(type),
          m_args(std::vector<std::string>())
{
        std::copy(args.begin(), args.end(), std::back_inserter(m_args));
}


std::string Command::arg1()
{
        if (m_args.size() > 0) {
                return m_args[0];
        }

        return std::string();
}

std::string Command::arg2()
{
        if (m_args.size() > 1)
        {
                return m_args[1];
        }

        return std::string();
}
