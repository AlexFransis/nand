#include "vm_command.h"
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>


VMCommand::VMCommand(const std::string &type, const std::string &command, const std::vector<std::string> &args)
        : m_command_type(type),
          m_command(command),
          m_args(std::vector<std::string>())
{
        std::copy(args.begin(), args.end(), std::back_inserter(m_args));
}


std::string VMCommand::arg1() const
{
        if (m_args.size() > 0) {
                return m_args[0];
        }

        return std::string();
}

std::string VMCommand::arg2() const
{
        if (m_args.size() > 1)
        {
                return m_args[1];
        }

        return std::string();
}
