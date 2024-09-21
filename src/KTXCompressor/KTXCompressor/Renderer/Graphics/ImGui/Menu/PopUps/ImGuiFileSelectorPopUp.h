//
// Created by ShaneMonck on 15/09/2024.
//

#ifndef KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H
#define KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H

#include "../../../../../Common.h"
#include "../../../../../Utils/ObserverPattern/Observable.h"

namespace KTXCompressor {

    class ImGuiFileSelectorPopUp {
    public:
        ImGuiFileSelectorPopUp();

        void Show();

    private:
        Observable<string> *selectedFile;

        bool initialShow = true;
        vector<string> fileNames;
        bool isOpen = false;

        string currentPath;
        char fileBufferName[1000];

        constexpr static const char *const FILE_SELECTOR_NAME = "ImGuiFileSelectorPopUp";

        static void GetFileNamesForCurrentPath(const string &currentPath, vector<string> &fileNames);

        static void RemoveLastFolder(char *path);

    public:
        Observable<string> *GetSelectedFileObservable() {
            return selectedFile;
        }
    };

} // KTXCompressor

#endif //KTX_COMPRESSOR_IMGUIFILESELECTORPOPUP_H
