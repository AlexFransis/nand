#ifndef GUARD_analyzer_h
#define GUARD_analyzer_h

#include <fstream>
#include "file_handler.h"

class Analyzer {
private:
        std::ifstream			m_ifstream;
        std::ofstream			m_ofstream;
        std::vector<io_paths>		m_io_paths;

public:
        Analyzer(const std::string &path, const std::string &input);
        ~Analyzer();

        void				begin();
};

#endif
