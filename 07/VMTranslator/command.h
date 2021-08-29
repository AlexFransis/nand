#ifndef GUARD_command_h
#define GUARD_command_h

#include <cstdint>
#include <string>
#include <vector>

class Command {
private:
        std::string 			m_type;
        std::vector<std::string>	m_args;

      public:
        Command(const std::string &type);
        Command(const std::string &type, const std::vector<std::string> &args);

        inline std::string 		command_type() 	const {return m_type;}
        std::string 			arg1();
        std::string 			arg2();
};

#endif
