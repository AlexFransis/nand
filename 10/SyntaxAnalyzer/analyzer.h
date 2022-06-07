#ifndef GUARD_analyzer_h
#define GUARD_analyzer_h

#include <fstream>
#include "file_handler.h"

class Analyzer {
private:
        std::ifstream			m_ifstream;
        std::ofstream			m_ofstream;
        std::filesystem::path 		m_input_path;
        INPUT_TYPE			m_input_type;
        std::vector<io_paths>		m_io_paths;

        std::string 			trim_comments(const std::string &line);
        std::string 			trim_ws(const std::string &line);

public:
        Analyzer(const std::string &path, const std::string &input);
        ~Analyzer();

        void				begin();
};

#endif
