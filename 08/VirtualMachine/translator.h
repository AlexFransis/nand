#ifndef GUARD_translator_h
#define GUARD_translator_h

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class Translator {
private:
        std::vector<std::filesystem::path> 	m_inputs;
        std::filesystem::path 			m_output;
        std::ifstream 				m_ifstream;
        std::ofstream 				m_ofstream;
        std::string 				generate_uuid() const;
        void 					write_bootstrap(std::unordered_map<std::string, std::string> &state);

      public:
        Translator(const std::vector<std::filesystem::path> &inputs, const std::filesystem::path &output);
        ~Translator();

        void begin();
};

#endif
