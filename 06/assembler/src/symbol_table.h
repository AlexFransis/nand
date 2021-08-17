#include <unordered_map>


class SymbolTable {
public:
        SymbolTable();

        bool 				contains(const std::string&);
        void				add(const std::string&);
        void				add(const std::string&, uint16_t);
        uint16_t			get_address(const std::string&);

private:
        uint16_t					m_stack_pointer;
        std::unordered_map<std::string, uint16_t>	m_symbol_table;
};
