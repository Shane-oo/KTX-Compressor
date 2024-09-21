//
// Created by ShaneMonck on 15/09/2024.
//

#include <imgui.h>
#include "ImGuiFileSelectorPopUp.h"

namespace KTXCompressor {

    // #region Private Methods

    void ImGuiFileSelectorPopUp::RemoveLastFolder(char *path) {
        size_t length = strlen(path);

        if (length == 0) return;  // Nothing to remove

        // Check for a trailing slash
        if (path[length - 1] == '/' || path[length - 1] == '\\') {
            // Remove the trailing slash first
            path[length - 1] = '\0';
        }

        // Find the last occurrence of the directory separator
        size_t pos = strrchr(path, '/') ? strrchr(path, '/') - path : strrchr(path, '\\') ? strrchr(path, '\\') - path
                                                                                          : std::string::npos;

        if (pos != std::string::npos) {
            // Truncate the string at the found position
            path[pos] = '\0';
        } else {
            // If there are no separators, clear the path
            path[0] = '\0';
        }
    }


    void ImGuiFileSelectorPopUp::GetFileNamesForCurrentPath(const string &currentPath, vector<string> &fileNames) {
        fileNames.clear();

        try {
            for (const auto &entry: filesystem::directory_iterator(currentPath)) {
                if (!entry.exists()) {
                    continue; // Skip inaccessible entries
                }

                auto fileName = entry.path().filename().string();
                if (entry.is_directory()) {
                    fileNames.push_back(fileName + "\\"); // Indicate it's a directory
                }

                if (fileName.ends_with(".png") || fileName.ends_with(".jpeg") || fileName.ends_with(".jpg")) {
                    fileNames.push_back(fileName);
                }
            }
        }
        catch (exception &e) {
            cerr << "ERROR" << e.what() << std::endl;
            // do nothing
        }

        cout << fileNames.size() << endl;
    }

    // #endregion

    // #region Public Methods

    void ImGuiFileSelectorPopUp::Show(string &selectedFile) {
        static bool initialShow = true;
        static vector<string> fileNames;
        static bool isOpen = false;

        static string currentPath;
        static char fileBufferName[1000];
        if (initialShow) {
            currentPath = filesystem::current_path().string();
            strncpy_s(fileBufferName, currentPath.c_str(), sizeof(fileBufferName) - 1);

            GetFileNamesForCurrentPath(fileBufferName, fileNames);
        }


        if (ImGui::Button("Select Image")) {
            isOpen = true; // Set isOpen to true when button is pressed
            ImGui::OpenPopup(FILE_SELECTOR_NAME);
        }

        ImGui::SetNextWindowSize(ImVec2(800.f, 600.f));
        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));

        if (ImGui::BeginPopupModal(FILE_SELECTOR_NAME,
                                   &isOpen,
                                   ImGuiWindowFlags_AlwaysAutoResize)) {

            ImGui::SetNextItemWidth(700.f);
            if (ImGui::InputText("Path", fileBufferName,
                                 IM_ARRAYSIZE(fileBufferName),
                                 ImGuiInputTextFlags_EnterReturnsTrue)) {
                GetFileNamesForCurrentPath(fileBufferName, fileNames);
            }

            if (ImGui::Button("..")) {
                RemoveLastFolder(fileBufferName);

                GetFileNamesForCurrentPath(fileBufferName, fileNames);
            }

            for (const auto &name: fileNames) {
                if (ImGui::Button(name.c_str())) {
                    if (name.ends_with("\\")) {
                        string directorySuffix = "\\" + name;

                        // Copying the directory name into fileBufferName
                        strncpy_s(fileBufferName,
                                  (fileBufferName + directorySuffix).c_str(),
                                  sizeof(fileBufferName) - 1);
                        fileBufferName[sizeof(fileBufferName) - 1] = '\0';

                        GetFileNamesForCurrentPath(fileBufferName, fileNames);

                        break; // messed with filenames list -> must break;
                    } else {
                        selectedFile = fileBufferName + name;
                        isOpen = false; //Close the modal
                    }
                }
            }

            ImGui::EndPopup();
        }

        initialShow = false;
    }


    // #endregion

} // KTXCompressor