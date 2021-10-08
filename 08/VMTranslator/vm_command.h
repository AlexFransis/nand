#ifndef GUARD_vm_command_h
#define GUARD_vm_command_h

#include <cstdint>
#include <string>
#include <vector>

class VMCommand {
private:
        std::string 			m_command_type;
        std::string 			m_command;
        std::vector<std::string>	m_args;

public:
        VMCommand(const std::string &type, const std::string &command);
        VMCommand(const std::string &type, const std::string &command, const std::vector<std::string> &args);

        inline std::string 		command_type() 	const {return m_command_type;}
        inline std::string 		command() 	const {return m_command;}
        std::string 			arg1() const;
        std::string 			arg2() const;
};

#endif
