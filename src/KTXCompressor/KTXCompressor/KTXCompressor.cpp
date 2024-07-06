// KTXCompressor.cpp : Defines the entry point for the application.
//

#include "KTXCompressor.h"

#include "Texture.h"
#include "Renderer/RendererApp.h"

using namespace std;

int main() {
    RendererApp rendererApp;
    try {
        /*
         * const string woodDiffuseFileName = "textures/wood_diffuse_4096x4096.png";
         * KTXCompressor::Texture texture = KTXCompressor::Texture(woodDiffuseFileName);
         */
        rendererApp.Run();
    }
    catch (const exception &e) {
        cerr << "An Exception Happened: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

