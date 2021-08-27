#include "address_table.h"

AddressTable::AddressTable()
        : m_stack_pointer(0x0100),
          m_var_pointer(0x0010),
          m_heap_pointer(0x0800),
          m_table({
                        {"SP", 			0x0000},
                        {"LCL", 		0x0001},
                        {"ARG", 		0x0002},
                        {"THIS", 		0x0003},
                        {"THAT", 		0x0004},
                        {"R0", 			0x0000},
                        {"R1", 			0x0001},
                        {"R2", 			0x0002},
                        {"R3", 			0x0003},
                        {"R4", 			0x0004},
                        {"R5", 			0x0005},
                        {"R6", 			0x0006},
                        {"R7", 			0x0007},
                        {"R8", 			0x0008},
                        {"R9", 			0x0009},
                        {"R10", 		0x000A},
                        {"R11", 		0x000B},
                        {"R12", 		0x000C},
                        {"R13", 		0x000D},
                        {"R14", 		0x000E},
                        {"R15", 		0x000F},
                        {"STATIC_VARS", 	0x0010},
                        {"STACK", 		0x0100},
                        {"HEAP", 		0x0800},
                        {"SCREEN", 		0x4000},
                        {"KBD", 		0x6000}
                })
{
}
