//
// Created by shane on 21/09/2024.
//

#include <imgui.h>
#include "ImGuiMainMenu.h"
#include "PopUps/ImGuiFileSelectorPopUp.h"

namespace KTXCompressor {

    // # region Public Methods

    void ImGuiMainMenu::Present(string &selectedFile) {
        // everything want render must be between ImGui::NewFrame() 
        // and ImGui::Render that happens in the ImGuiRenderPass

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        ImGui::Begin(MAIN_MENU_NAME, nullptr, windowFlags);
        // todo make main window not draggable or moveable...

        ImGui::SetWindowSize(ImVec2(250.f, 250.f));
        ImGui::SetWindowPos(ImVec2(0.f, 0.f));

        ImGuiFileSelectorPopUp::Show(selectedFile);
        if (!selectedFile.empty()) {
            cout << selectedFile << endl;
        }

        ImGui::End(); // ends main window
    }

    // #endregion

} // KTXCompressor