//
// Created by shane on 21/09/2024.
//

#ifndef KTXCOMPRESSOR_IMGUIMAINMENU_H
#define KTXCOMPRESSOR_IMGUIMAINMENU_H

#include "../../../../Common.h"
#include "PopUps/ImGuiFileSelectorPopUp.h"

namespace KTXCompressor {

    struct MainMenuObservables {
        Observable<string> *ImageToCompressSelected;
    };

    class ImGuiMainMenu {
    public:
        ImGuiMainMenu();

        ~ImGuiMainMenu();

        void Present();

    private:
        ImGuiFileSelectorPopUp *fileSelectorPopUp;
        MainMenuObservables mainMenuObservables;

        constexpr static const char *const MAIN_MENU_NAME = "Main Menu";

    public:
        MainMenuObservables GetMainMenuObservable() {
            return mainMenuObservables;
        }

    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_IMGUIMAINMENU_H
