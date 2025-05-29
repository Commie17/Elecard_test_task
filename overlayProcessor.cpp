#include "overlayProcessor.h"

using namespace std;


void OverlayProcessor::overlayImageOnVideo(const string& imagePath, const string& videoPath)
{
    ImageBmp* imageBmp = new ImageBmp();
    imageBmp->setNewImageFromFile(imagePath);

    MatrixYUV420 imageYuv = Converter::convertRGB24ToYUV420(imageBmp->getPixelsMatrix());

    //ниже производим создание пустого файла подходящего размера для записи результата
    string resultFileName = "overlay " + videoPath;
    size_t sizeOfInputFile = get_video_file_size(videoPath);

    ofstream resultFile(resultFileName, ios::binary | ios::trunc);
    if (!resultFile.is_open())
        throw runtime_error("Cannot create output file");

    resultFile.seekp(sizeOfInputFile - 1);  // Переходим к последнему байту
    char zero = 0;
    resultFile.write(&zero, 1); // Записываем 1 пустой байт
    resultFile.close();

    Yuv420File videoForOverlay = Yuv420File(videoPath, 1920, 1080);
    Yuv420File videoWithResult = Yuv420File(resultFileName, 1920, 1080);

    size_t numOfThreads = thread::hardware_concurrency();
    size_t frameQuantity = sizeOfInputFile / videoForOverlay.frameSize;
    vector<thread> threads;

    for (size_t i = 0; i < numOfThreads; ++i)
    {
        size_t startIndex = i * frameQuantity / numOfThreads;
        size_t endIndex = (i + 1) * frameQuantity / numOfThreads;
        
        if (i == numOfThreads - 1)
            endIndex = frameQuantity;
        
        threads.push_back(thread(OverlayProcessor::overlay_in_thread, cref(imageYuv), cref(videoForOverlay), cref(videoWithResult), startIndex, endIndex));
    }

    for (auto& it : threads)
    {
        it.join();
    }
    
    delete imageBmp;
}


size_t OverlayProcessor::get_video_file_size(const std::string& videoPath)
{
    ifstream file(videoPath.c_str(), ios::binary | ios::ate);
    if (!file.is_open()) throw exception("Unable to open file");

    size_t size = file.tellg();
    file.close();
    
    return size;
}


void OverlayProcessor::overlay_in_thread(MatrixYUV420 imageYuv, const Yuv420File& rawVideo, const Yuv420File& resultVideo, size_t startFrameIndex, size_t endFrameIndex)
{
    VideoYuv420Reader* reader = new VideoYuv420Reader();
    VideoYuv420Writer* writer = new VideoYuv420Writer();

    reader->setNewVideoFile(rawVideo);
    reader->setPointerToFrameNumber(startFrameIndex);
    
    writer->setNewWritingFile(resultVideo);
    writer->setPointerToFrameNumber(startFrameIndex);

    size_t currentFrameIndex = startFrameIndex;

    while (reader->hasNextFrame() && currentFrameIndex < endFrameIndex)
    {
        MatrixYUV420 frame = reader->getNextFrame();
        frame = frame / 2 + imageYuv / 2;
        writer->writeFrame(frame);
        ++currentFrameIndex;
    }

    delete reader;
    delete writer;
}
