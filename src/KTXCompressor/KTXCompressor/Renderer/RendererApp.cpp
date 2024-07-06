//
// Created by Shane Monck on 6/07/2024.
//


#include "RendererApp.h"



// #region Private Methods

void RendererApp::InitVulkan() {
    window = new Window();
}

void RendererApp::MainLoop() {
    while (!window->GetWindowShouldClose()) {
        cout << "We should be running" << endl;
    }
}



// #endregion

// #region Public Methods

void RendererApp::Run() {
    InitVulkan();
    MainLoop();
}






// #endregion
