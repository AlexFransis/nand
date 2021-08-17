#include <fstream>


enum Command {
        INVALID,
	A_COMMAND, // \@xxx where xxx is a symbol or decimal
	C_COMMAND, // dest=comp;jump
	L_COMMAND // (xxx) where xxx is a label

};


class Parser {
	private:
		std::ifstream& 			m_fstream;
		std::string 			m_line;
		std::string 			trim_ws(const std::string&);
		std::string 			trim_comments(const std::string&);
                bool 				is_valid_command(const std::string&);
                bool 				is_comment(const std::string&);
                bool 				is_ws(const std::string&);


	public:
		Parser(std::ifstream&);
		~Parser();

		bool 				has_more_commands() const;
		void 				advance();
		std::string			current_line() const;
		Command 			command_type(const std::string&);
		std::string 			symbol(const std::string&);
		std::string 			dest(const std::string&);
		std::string 			comp(const std::string&);
		std::string 			jump(const std::string&);
};
