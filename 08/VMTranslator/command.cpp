#include "command.h"
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>


Command::Command(const std::string &type, const std::string &name, const std::vector<std::string> &args)
        : m_type(type),
          m_name(name),
          m_args(args)
{
}
