#pragma once
#include "dataContainers.h"

class VideoYuv420Reader
{
public:
    VideoYuv420Reader() = default;

    void setNewVideoFile(const Yuv420File& newVideoFile);
    void setPointerToFrameNumber(size_t frameNumber);
    const MatrixYUV420& getNextFrame();
    const size_t& getFrameQuantity() const;
    const size_t& getFrameNumber() const;
    bool hasNextFrame() const;
    
private:
    std::ifstream video_file_stream;
    size_t frame_number;
    size_t frame_quantity;

    Yuv420File current_file = Yuv420File("result.yuv", 1920, 1080);
    MatrixYUV420 current_frame = MatrixYUV420(1920, 1080);
};