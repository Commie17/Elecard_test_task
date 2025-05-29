#include "videoYuv420Reader.h"

using namespace std;


void VideoYuv420Reader::setPointerToFrameNumber(size_t frameNumber)
{
    video_file_stream.seekg(frameNumber * current_file.frameSize);
    frame_number = frameNumber;
}


void VideoYuv420Reader::setNewVideoFile(const Yuv420File& newVideoFile)
{
    current_file = newVideoFile;
    if (video_file_stream.is_open()) video_file_stream.close();
    
    video_file_stream.open(current_file.path, ios::binary | ios::ate);
    if (!video_file_stream.is_open()) throw runtime_error("Could not open file " + current_file.path);
    
    frame_quantity = video_file_stream.tellg() / current_file.frameSize;
    frame_number = 0;

    video_file_stream.seekg(ios::beg);
}


const MatrixYUV420& VideoYuv420Reader::getNextFrame()
{
    if (!hasNextFrame()) throw runtime_error("No more frames to read");
    
    if (current_frame.Y.size() != current_file.width * current_file.height)
    {
        current_frame = MatrixYUV420(current_file.width, current_file.height);
    }
    
    video_file_stream.read(reinterpret_cast<char*>(current_frame.Y.data()), current_file.width * current_file.height);
    if (!video_file_stream) throw runtime_error("Failed to read Y component");
    
    video_file_stream.read(reinterpret_cast<char*>(current_frame.U.data()), (current_file.width / 2) * (current_file.height / 2));
    if (!video_file_stream) throw runtime_error("Failed to read U component");
    
    video_file_stream.read(reinterpret_cast<char*>(current_frame.V.data()), (current_file.width / 2) * (current_file.height / 2));
    if (!video_file_stream) throw runtime_error("Failed to read V component");

    ++frame_number;
    
    return current_frame;
}


bool VideoYuv420Reader::hasNextFrame() const
{
    return frame_number  < frame_quantity;
}


const size_t& VideoYuv420Reader::getFrameQuantity() const
{
    return frame_quantity;
}


const size_t& VideoYuv420Reader::getFrameNumber() const
{
    return frame_number;
}



