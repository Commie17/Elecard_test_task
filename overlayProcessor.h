#pragma once

#include <string>
#include <fstream>
#include <thread>
#include <vector>
#include "imageBmp.h"
#include "converter.h"
#include "videoYuv420Reader.h"
#include "videoYuv420Writer.h"

class OverlayProcessor
{
public:
    static void overlayImageOnVideo(const std::string& imagePath, const std::string& videoPath);
    static void overlay_in_thread(MatrixYUV420 imageYuv, const Yuv420File& rawVideo, const Yuv420File& resultVideo, size_t startFrameIndex, size_t endFrameIndex);
private:
    static size_t get_video_file_size(const std::string& videoPath);
    
};
