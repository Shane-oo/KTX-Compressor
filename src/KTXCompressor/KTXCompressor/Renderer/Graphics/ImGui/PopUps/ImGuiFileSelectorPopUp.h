//
// Created by ShaneMonck on 15/09/2024.
//

#ifndef KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H
#define KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H

#include "../../../../Common.h"

namespace KTXCompressor {

    class ImGuiFileSelectorPopUp {
    public:
        ImGuiFileSelectorPopUp();


    private:
        const char *const popUpName = "ImGuiFileSelectorPopUp";
        bool isOpen = false;
        string currentPath;
        vector<string> fileNames;
        void RegisterModalPopUpDialog(bool* pOpen);

    public:
        void Show();
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H
