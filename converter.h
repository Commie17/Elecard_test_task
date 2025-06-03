#pragma once
#include "dataContainers.h"
#include <thread>


class Converter
{
public:
    static MatrixYUV420 convertRGB24ToYUV420(const MatrixRGB& rgb24Matrix);
private:
    static void compute_yuv_in_thread(const MatrixRGB& rgb24Matrix,
        std::vector<std::vector<uint8_t>>& vecY,
        std::vector<std::vector<uint8_t>>& vecU,
        std::vector<std::vector<uint8_t>>& vecV,
        size_t startIndex,
        size_t endIndex);
};
