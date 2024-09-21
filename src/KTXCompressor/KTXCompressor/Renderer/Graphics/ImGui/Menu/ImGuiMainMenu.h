//
// Created by shane on 21/09/2024.
//

#ifndef KTXCOMPRESSOR_IMGUIMAINMENU_H
#define KTXCOMPRESSOR_IMGUIMAINMENU_H

#include "../../../../Common.h"

namespace KTXCompressor {

    class ImGuiMainMenu {
    public:
        static void Present(string &selectedFile);

    private:
        constexpr static const char *const MAIN_MENU_NAME = "Main Menu";
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_IMGUIMAINMENU_H
