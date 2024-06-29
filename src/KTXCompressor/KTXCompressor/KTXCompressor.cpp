// KTXCompressor.cpp : Defines the entry point for the application.
//

#include "KTXCompressor.h"

#include <ktx.h>
#include <OpenImageIO/imageio.h>

using namespace std;
using namespace OIIO;

int main() {

    const std::string fileName = "textures/wood_diffuse_4096x4096.png";

    auto imageInput = ImageInput::open(fileName);
    if (!imageInput) {
        throw std::runtime_error("Could not open file with name" + fileName);
    }

    const ImageSpec &imageSpec = imageInput->spec();

    std::cout << "image width: " << imageSpec.width << "image height: " << imageSpec.height << std::endl;

    imageInput->close();

    return 0;
}

