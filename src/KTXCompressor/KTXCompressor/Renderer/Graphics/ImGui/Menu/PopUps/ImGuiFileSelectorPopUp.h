//
// Created by ShaneMonck on 15/09/2024.
//

#ifndef KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H
#define KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H

#include "../../../../../Common.h"

namespace KTXCompressor {

    class ImGuiFileSelectorPopUp {
    public:
        static void Show(string &selectedFile);

    private:
        constexpr static const char *const FILE_SELECTOR_NAME = "ImGuiFileSelectorPopUp";

        static void GetFileNamesForCurrentPath(const string &currentPath, vector<string> &fileNames);

        static void RemoveLastFolder(char *path);
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H
