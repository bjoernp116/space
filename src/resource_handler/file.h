#pragma once

#include <fstream>
#include <vector>
class File {
    private:
        std::string path; 
        std::fstream file;
        bool open;

    public:
        File(const char* path);
        File(std::string path);

        void write_line(const char* line);

        const std::string read() const;
        const std::vector<std::string> read_lines() const;
};
