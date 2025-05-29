#pragma once
#include <fstream>
#include <string>
#include "dataContainers.h"

class VideoYuv420Writer {
public:
    VideoYuv420Writer() = default;
    ~VideoYuv420Writer();

    void setNewWritingFile(const Yuv420File& newVideoFile);
    void setPointerToFrameNumber(size_t frameNumber);
    void writeFrame(const MatrixYUV420& frame);

private:

    Yuv420File current_file = Yuv420File("out.yuv", 1920, 1080);
    std::ofstream file;
    size_t frame_number = 0;
};

