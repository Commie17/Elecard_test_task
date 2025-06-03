#pragma once
#include <string>
#include "imageBmp.h"

class Validator
{
public:
    static bool isImageResolutionFit(const std::string& imageFile, size_t width, size_t height);
    static bool isVideoResolutionFit(const std::string& videoFile, size_t width, size_t height);
};
