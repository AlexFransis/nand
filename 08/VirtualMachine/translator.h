#ifndef GUARD_translator_h
#define GUARD_translator_h

#include <filesystem>
#include <fstream>
#include <vector>

class Translator {
private:
        std::vector<std::filesystem::path> 	m_inputs;
        std::filesystem::path 			m_output;
        std::ifstream 				m_ifstream;
        std::ofstream 				m_ofstream;

      public:
        Translator(const std::vector<std::filesystem::path> &inputs, const std::filesystem::path &output);
        ~Translator();

        void begin();
};

#endif
