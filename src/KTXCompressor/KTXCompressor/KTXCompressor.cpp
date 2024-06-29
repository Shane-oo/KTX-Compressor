// KTXCompressor.cpp : Defines the entry point for the application.
//

#include "KTXCompressor.h"

#include "Texture.h"

using namespace std;

int main() {
    const string woodDiffuseFileName = "textures/wood_diffuse_4096x4096.png";

    KTXCompressor::Texture texture = KTXCompressor::Texture(woodDiffuseFileName);

    /*auto ktxTexture = texture.GetKtxTexture();
    if (ktxTexture) {
        cout << "Got A Ktx Texture!" << endl;
    } else {
        cout << "No Ktx Texture :(" << endl;
    }*/

    return 0;
}

