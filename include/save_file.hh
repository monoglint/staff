#pragma once

#include <string>

struct file_save_info {
    bool saved = false;
    std::string file_path = "";
};

file_save_info request_save_file();