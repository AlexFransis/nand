#include "command.h"
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>


Command::Command(const std::string &type, const std::string &command, const std::vector<std::string> &args)
        : m_command_type(type),
          m_command(command),
          m_args(args)
{
}
