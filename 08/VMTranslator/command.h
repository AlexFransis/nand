#ifndef GUARD_command_h
#define GUARD_command_h

#include <cstdint>
#include <string>
#include <vector>

class Command {
private:
        std::string 			m_command_type;
        std::string 			m_command_name;
        std::vector<std::string>	m_args;

public:
        Command(const std::string &type, const std::string &command, const std::vector<std::string> &args);

        inline std::string 		type() 	const {return m_command_type;}
        inline std::string 		name() 	const {return m_command_name;}
        inline std::string 		arg1() const { return m_args.size() > 0 ? m_args[0] : std::string();}
        inline std::string 		arg2() const { return m_args.size() > 1 ? m_args[1] : std::string();}
};

#endif
