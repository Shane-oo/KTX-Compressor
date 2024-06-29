// KTXCompressor.cpp : Defines the entry point for the application.
//

#include "KTXCompressor.h"

#include "Texture.h"

using namespace std;

int main() {
    try {
        const string woodDiffuseFileName = "textures/wood_diffuse_4096x4096.png";

        KTXCompressor::Texture texture = KTXCompressor::Texture(woodDiffuseFileName);

        /*auto myKtxTexture = texture.GetKtxTexture();
        if (myKtxTexture) {
            cout << "Got A Ktx Texture!" << endl;
        } else {
            cout << "No Ktx Texture :(" << endl;
        }*/
    }
    catch (const std::runtime_error &e) {
        std::cerr << "Caught a runtime_error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught an unknown exception" << std::endl;
    }

    return 0;
}

