#ifndef GUARD_file_handler_h
#define GUARD_file_handler_h

#include <filesystem>
#include <string>
#include <vector>

enum class INPUT_TYPE {
        DIR,
        FILE
};

typedef std::pair<std::filesystem::path, std::filesystem::path> io_paths;

class FileHandler {
private:
        std::filesystem::path 			m_input_path;
        INPUT_TYPE				m_input_type;

        std::vector<std::filesystem::path>	traverse_dir(const std::filesystem::path &dir, const std::string &ext);
        std::vector<std::filesystem::path>	scan_files(INPUT_TYPE input, const std::string &ext);
        std::vector<io_paths>	construct_output_files(std::vector<std::filesystem::path> &inputs,
                                                                       const std::string &ext);

public:
        FileHandler(const std::string &path, std::string input);

        std::vector<io_paths> get_io_paths();
};

#endif
