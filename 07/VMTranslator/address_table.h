
#include <cstdint>
#include <string>
#include <unordered_map>

class AddressTable {
private:
        uint16_t m_stack_pointer;
        uint16_t m_var_pointer;
        uint16_t m_heap_pointer;
        std::unordered_map<std::string, uint16_t> m_table;

public:
        AddressTable();
        uint16_t 			get_base_address(const std::string &location);
        inline void			increment_sp() {++m_stack_pointer;};
        inline void			decrement_sp() {--m_stack_pointer;};
};
