#include "save_file.hh"

#include <windows.h>
#include <iostream>

file_save_info request_save_file() {
    OPENFILENAME ofn;
    TCHAR sz_file[256] = {0};

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = sz_file;
    ofn.nMaxFile = sizeof(sz_file) / sizeof(TCHAR);
    ofn.lpstrFilter = TEXT("CSV File\0*.csv\0");
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = TEXT("csv");

    return {
        GetSaveFileName(&ofn) == TRUE,
        std::string(ofn.lpstrFile)
    };
}