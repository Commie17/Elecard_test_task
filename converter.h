#pragma once
#include "dataContainers.h"


class Converter
{
public:
    static MatrixYUV420 convertRGB24ToYUV420(const MatrixRGB& rgb24Matrix);
};
