#ifndef GUARD_file_handler_h
#define GUARD_file_handler_h

#include <filesystem>
#include <string>
#include <vector>

enum class INPUT_TYPE {
        DIR,
        FILE
};

class FileHandler {
private:
        std::filesystem::path 			m_input_path;
        INPUT_TYPE				m_input_type;

        std::vector<std::filesystem::path>	traverse_dir(const std::filesystem::path &dir, const std::string &ext);
        std::vector<std::filesystem::path>	scan_files(INPUT_TYPE input, const std::string &ext);
        std::filesystem::path			construct_output_file(INPUT_TYPE input, const std::string &ext);

public:
        FileHandler(const std::string &path, INPUT_TYPE input);

        std::pair<std::vector<std::filesystem::path>, std::filesystem::path> get_io_paths(); // <inputs, output>
};

#endif
