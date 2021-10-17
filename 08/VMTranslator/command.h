#ifndef GUARD_command_h
#define GUARD_command_h

#include <cstdint>
#include <string>
#include <vector>

class Command {
private:
        std::string 			m_command_type;
        std::string 			m_command;
        std::vector<std::string>	m_args;

public:
        Command(const std::string &type, const std::string &command, const std::vector<std::string> &args);

        inline std::string 		command_type() 	const {return m_command_type;}
        inline std::string 		command() 	const {return m_command;}
        inline std::string 		arg1() const { return m_args.size() > 0 ? m_args[0] : std::string();}
        inline std::string 		arg2() const { return m_args.size() > 1 ? m_args[1] : std::string();}
};

#endif
