#include "validator.h"

using namespace std;


bool Validator::isImageResolutionFit(const string& imageFile, size_t width, size_t height)
{
    ImageBmp image;
    image.setNewImageFromFile(imageFile);
    
    return image.getPixelsMatrix().size() == height && image.getPixelsMatrix()[0].size() == width;
}


bool Validator::isVideoResolutionFit(const std::string& videoFile, size_t width, size_t height)
{
    ifstream videoFileStream(videoFile.c_str(), ios::binary | ios::ate);
    
    if (!videoFileStream.is_open())
        throw runtime_error("Error opening video file");

    size_t fileSize = videoFileStream.tellg();
    size_t frameExpectedSize = 3 * width * height / 2;
    
   return fileSize % frameExpectedSize == 0;
}
