//
// Created by shane on 21/09/2024.
//

#include <imgui.h>
#include "ImGuiMainMenu.h"

namespace KTXCompressor {

    // #region Constructors

    ImGuiMainMenu::ImGuiMainMenu() {
        fileSelectorPopUp = new ImGuiFileSelectorPopUp();

        mainMenuObservables = {
                fileSelectorPopUp->GetSelectedFileObservable()
       };
    }

    // #endregion

    // #region Destructors

    ImGuiMainMenu::~ImGuiMainMenu() {
        delete fileSelectorPopUp;
    }

    // #endregion

    // #region Public Methods

    void ImGuiMainMenu::Present() {
        // everything want render must be between ImGui::NewFrame() 
        // and ImGui::Render that happens in the ImGuiRenderPass

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        ImGui::Begin(MAIN_MENU_NAME, nullptr, windowFlags);
        // todo make main window not draggable or moveable...

        ImGui::SetWindowSize(ImVec2(250.f, 250.f));
        ImGui::SetWindowPos(ImVec2(0.f, 0.f));

        fileSelectorPopUp->Show();

        ImGui::End(); // ends main window
    }

    // #endregion

} // KTXCompressor