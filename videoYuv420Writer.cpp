#include "videoYuv420Writer.h"
#include <stdexcept>

using namespace std;


VideoYuv420Writer::~VideoYuv420Writer()
{
    if (file.is_open()) file.close();
}


void VideoYuv420Writer::setNewWritingFile(const Yuv420File& newVideoFile)
{
    if (file.is_open()) file.close();
    file.open(newVideoFile.path, ios::binary);
    if (!file.is_open()) throw runtime_error("Error opening file");
}


void VideoYuv420Writer::writeFrame(const MatrixYUV420& frame)
{
    if (!file.is_open()) throw runtime_error("Error opening file");
    
    file.write(reinterpret_cast<const char*>(frame.Y.data()), frame.Y.size());
    file.write(reinterpret_cast<const char*>(frame.U.data()), frame.U.size());
    file.write(reinterpret_cast<const char*>(frame.V.data()), frame.V.size());

    if (!file) throw runtime_error("Failed to write frame data to file");
}


void VideoYuv420Writer::setPointerToFrameNumber(size_t frameNumber)
{
    if (!file.is_open()) throw runtime_error("Error opening file");

    file.seekp(current_file.frameSize * frameNumber);
    frame_number = frameNumber;
}
