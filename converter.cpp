#include "converter.h"

using namespace std;


MatrixYUV420 Converter::convertRGB24ToYUV420(const MatrixRGB& rgb24Matrix)
{
    const size_t height = rgb24Matrix.size();
    const size_t width = rgb24Matrix[0].size();

    if (width % 2 != 0 || height % 2 != 0) throw invalid_argument("Image dimensions must be even for YUV420 format.");
    
    MatrixYUV420 yuv(width, height);

    // Первый проход: рассчитать Y, U, V в полном разрешении
    vector<vector<uint8_t>> fullY(height, vector<uint8_t>(width));
    vector<vector<uint8_t>> fullU(height, vector<uint8_t>(width));
    vector<vector<uint8_t>> fullV(height, vector<uint8_t>(width));

    vector<thread> threads;
    size_t threadsNum = thread::hardware_concurrency();
    
    for (size_t i = 0; i < threadsNum; ++i)
    {
        size_t startIndex = i * height / threadsNum;
        size_t endIndex = (i + 1) * height / threadsNum;

        if (i == threadsNum - 1)
            endIndex = height;

        threads.push_back(thread(
            compute_yuv_in_thread,
            cref(rgb24Matrix),
            ref(fullY),
            ref(fullU),
            ref(fullV),
            startIndex,
            endIndex));
    }

    for (auto& thread : threads)
    {
        thread.join();
    }
    
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            yuv.Y[y * width + x] = fullY[y][x];
        }
    }

    // Второй проход: субдискретизация U и V по 2x2 пикселя
    size_t uvWidth = width / 2;
    size_t uvHeight = height / 2;

    for (size_t y = 0; y < uvHeight; ++y)
    {
        for (size_t x = 0; x < uvWidth; ++x)
        {
            int sumU = 0, sumV = 0;

            for (size_t dy = 0; dy < 2; ++dy)
            {
                for (size_t dx = 0; dx < 2; ++dx)
                {
                    sumU += fullU[y * 2 + dy][x * 2 + dx];
                    sumV += fullV[y * 2 + dy][x * 2 + dx];
                }
            }

            yuv.U[y * uvWidth + x] = static_cast<uint8_t>(sumU / 4);
            yuv.V[y * uvWidth + x] = static_cast<uint8_t>(sumV / 4);
        }
    }

    return yuv;
}

void Converter::compute_yuv_in_thread(const MatrixRGB& rgb24Matrix,
        std::vector<std::vector<uint8_t>>& vecY,
        std::vector<std::vector<uint8_t>>& vecU,
        std::vector<std::vector<uint8_t>>& vecV,
        size_t startIndex,
        size_t endIndex)
{
    for (size_t y = startIndex; y < endIndex; ++y)
    {
        for (size_t x = 0; x < vecY[0].size(); ++x)
        {
            uint8_t R = rgb24Matrix[y][x][0];
            uint8_t G = rgb24Matrix[y][x][1];
            uint8_t B = rgb24Matrix[y][x][2];

            float fY =  0.299f * R + 0.587f * G + 0.114f * B;
            float fU = -0.169f * R - 0.331f * G + 0.5f   * B + 128.0f;
            float fV =  0.5f   * R - 0.419f * G - 0.081f * B + 128.0f;

            vecY[y][x] = static_cast<uint8_t>(round(fY));
            vecU[y][x] = static_cast<uint8_t>(round(fU));
            vecV[y][x] = static_cast<uint8_t>(round(fV));
        }
    }
}
