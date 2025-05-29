#include "imageBmp.h"

using namespace std;


const MatrixRGB& ImageBmp::getPixelsMatrix() const
{
    return pixel_matrix;
}


void ImageBmp::setNewImageFromFile(const std::string& fileName)
{
    BmpFile bmpFileData = open_file(fileName);
    pixel_matrix = get_bmp_pixel_matrix(bmpFileData);
}


BmpFile ImageBmp::open_file(const std::string& pathToFile) const
{
    ifstream file(pathToFile, ios::binary);
    
    BITMAPFILEHEADER fileHeader{};
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (!file) throw runtime_error("Failed to read BMP file header");
    if (fileHeader.bfType != 0x4D42) throw runtime_error("Not a BMP file");

    BITMAPINFOHEADER infoHeader{};
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    if (!file) throw runtime_error("Failed to read BMP info header");
    
    if (infoHeader.biWidth <= 0 || abs(infoHeader.biHeight) <= 0) throw runtime_error("Invalid BMP dimensions");

    return BmpFile(file, fileHeader, infoHeader);
}


MatrixRGB ImageBmp::get_bmp_pixel_matrix(BmpFile& bmpFile) const
{
    bmpFile.file.seekg(bmpFile.fileHeader.bfOffBits, ios::beg);
    if (!bmpFile.file) throw runtime_error("Failed to seek to pixel data");

    int width = bmpFile.infoHeader.biWidth;
    int height = abs(bmpFile.infoHeader.biHeight);
    bool topDown = bmpFile.infoHeader.biHeight < 0;

    MatrixRGB matrix(height, vector<Vec3_uint8>(width));

    const size_t rowSizeInBytes = ((width * 3 + 3) / 4) * 4;
    vector<uint8_t> rowData(rowSizeInBytes);

    for (int row = 0; row < height; ++row) {
        bmpFile.file.read(reinterpret_cast<char*>(rowData.data()), rowSizeInBytes);
        if (!bmpFile.file) throw runtime_error("Failed to read pixel data");

        int dstRow = topDown ? row : (height - 1 - row);

        for (int col = 0; col < width; ++col) {
            uint8_t B = rowData[col * 3 + 0];
            uint8_t G = rowData[col * 3 + 1];
            uint8_t R = rowData[col * 3 + 2];

            matrix[dstRow][col] = { R, G, B };
        }
    }

    return matrix;
}

