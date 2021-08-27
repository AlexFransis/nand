#ifndef GUARD_command_h
#define GUARD_command_h

#include <cstdint>
#include <string>

enum COMMAND_TYPE {
        NOT_INIT = -1,
        INVALID = 0,
        C_ARITHMETIC,
        C_PUSH,
        C_POP,
        C_LABEL,
        C_GOTO,
        C_IF,
        C_FUNCTION,
        C_RETURN,
        C_CALL
};

class Command {
private:
        std::string			m_command;
        COMMAND_TYPE 			m_type;
        std::string 			m_arg1;
        std::string 			m_arg2;
        void 				parse_str(const std::string &in_str,
                                                  COMMAND_TYPE &out_type,
                                                  std::string &out_arg1,
                                                  std::string &out_arg2);

public:
        Command(std::string);
        Command();

        inline COMMAND_TYPE 		command_type() 	const {return m_type;}
        inline std::string 		arg1() 		const {return m_arg1;}
        inline std::string 		arg2() 		const {return m_arg2;}
};

#endif
