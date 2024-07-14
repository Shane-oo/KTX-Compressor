//
// Created by Shane Monck on 6/07/2024.
//


#include "RendererApp.h"

namespace KTXCompressor {

    // #region Private Methods

    void RendererApp::MainLoop() {
        while (!window->GetWindowShouldClose()) {
            //cout << "We should be running" << endl;
        }
    }

    // #endregion

    // #region Constructors

    RendererApp::RendererApp() {
        window = new Window();
        instance = new Instance();
    }

    // #endregion

    // #region Destructors

    RendererApp::~RendererApp() {
        delete instance;
        delete window;
    }

    // #endregion

    // #region Public Methods

    void RendererApp::Run() {
        MainLoop();
    }


    // #endregion

} // KTXCompressor