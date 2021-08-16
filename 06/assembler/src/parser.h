#include <fstream>


enum Command {
	A_COMMAND, // @xxx where xxx is a symbol or decimal
	C_COMMAND, // dest=comp;jump
	L_COMMAND // (xxx) where xxx is as symbol

};


class Parser {
	private:
		std::ifstream& 			m_fstream;
		std::string 			m_line;
		std::string				m_file_name;

		std::string 			trim_ws(const std::string&);
		std::string 			trim_comments(const std::string&);

	public:
		Parser(std::ifstream&, const std::string&);
		~Parser();

		bool 					has_more_commands() const;
		void 					advance();
		std::string				current_line() const;
		Command 				command_type(const std::string&);
		std::string 			symbol(const std::string&);
		std::string 			dest(const std::string&);
		std::string 			comp(const std::string&);
		std::string 			jump(const std::string&);
};
