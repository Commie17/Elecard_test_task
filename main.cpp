#include "overlayProcessor.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <image.bmp> <video.yuv>" << std::endl;
        return 1;
    }

    const std::string imagePath = argv[1];
    const std::string videoPath = argv[2];

    try {
        OverlayProcessor::overlayImageOnVideo(imagePath, videoPath);
        std::cout << "Overlay complete!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}