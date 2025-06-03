#include "overlayProcessor.h"
#include "validator.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cerr << "Wrong arguments, use the same order: " << " <image.bmp> <video.yuv> <width> <height>" << std::endl;
        return 1;
    }

    const std::string imagePath = argv[1];
    const std::string videoPath = argv[2];
    
    size_t width = std::stoul(argv[3]);
    size_t height = std::stoul(argv[4]);

    if (!Validator::isImageResolutionFit(imagePath, width, height))
    {
        std::cerr << "Resolution of image is not valid" << std::endl;
        return 1;
    }
    
    if (!Validator::isVideoResolutionFit(videoPath, width, height))
    {
        std::cerr << "Resolution of video is not valid" << std::endl;
        return 1;
    }

    try {
        OverlayProcessor::overlayImageOnVideo(imagePath, videoPath, width, height);
        std::cout << "Overlay complete!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
