#include <cstdint>
#include <string>
#include <unordered_map>

class Code {
	private:
		std::unordered_map<std::string, uint8_t> dest_table;
		std::unordered_map<std::string, uint8_t> comp_table;
		std::unordered_map<std::string, uint8_t> jump_table;

	public:
		Code();
		uint8_t				dest(const std::string&);
		uint8_t				comp(const std::string&);
		uint8_t				jump(const std::string&);
		uint16_t			immediate(const std::string&);
};
