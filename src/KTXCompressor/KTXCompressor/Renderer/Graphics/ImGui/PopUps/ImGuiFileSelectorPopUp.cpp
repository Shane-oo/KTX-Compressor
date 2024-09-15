//
// Created by ShaneMonck on 15/09/2024.
//

#include <imgui.h>
#include "ImGuiFileSelectorPopUp.h"

namespace KTXCompressor {

    // #region Private Methods


    void ImGuiFileSelectorPopUp::RegisterModalPopUpDialog(bool *pOpen) {
        if (ImGui::BeginPopupModal(popUpName,
                                   pOpen,
                                   ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Current Path: %s", currentPath.c_str());

            // List the files
            for (const auto &name: fileNames) {
                ImGui::Text("%s", name.c_str());
            }

            ImGui::EndPopup();
        }
    }

    // #endregion

    // #region Constructors

    ImGuiFileSelectorPopUp::ImGuiFileSelectorPopUp() {
        currentPath = filesystem::current_path().string();
        for (const auto &entry: filesystem::directory_iterator(currentPath)) {
            fileNames.push_back(entry.path().filename().string());
        }
    }

    void ImGuiFileSelectorPopUp::Show() {
        if (ImGui::Button("Show File Selector")) {
            isOpen = true; // Set isOpen to true when button is pressed
            ImGui::OpenPopup(popUpName);
        }

        RegisterModalPopUpDialog(&isOpen);
    }

    // #endregion

} // KTXCompressor