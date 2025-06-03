#pragma once
#include <array>
#include <vector>
#include <fstream>
#include <algorithm>


typedef std::array<uint8_t, 3> Vec3_uint8;
typedef std::vector<std::vector<Vec3_uint8>> MatrixRGB;


struct MatrixYUV420
{
    std::vector<uint8_t> Y;
    std::vector<uint8_t> U; 
    std::vector<uint8_t> V;

    size_t width;
    size_t height;

    MatrixYUV420(size_t width, size_t height)
       : width(width), height(height),
         Y(width * height),
         U((width / 2) * (height / 2)),
         V((width / 2) * (height / 2)) {}

    MatrixYUV420 operator/(size_t number)
    {
        MatrixYUV420 result(width, height);

        for (size_t i = 0; i < Y.size(); ++i)
            result.Y[i] = Y[i] / number;

        for (size_t i = 0; i < U.size(); ++i)
            result.U[i] = U[i] / number;

        for (size_t i = 0; i < V.size(); ++i)
            result.V[i] = V[i] / number;

        return result;
    }

    MatrixYUV420 operator+(const MatrixYUV420& rhs)
    {
        if (Y.size() != rhs.Y.size() || U.size() != rhs.U.size() || V.size() != rhs.V.size())
        {
            throw std::invalid_argument("Matrix sizes do not match for addition.");
        }
    
        MatrixYUV420 result(width, height);

        for (size_t i = 0; i < Y.size(); ++i)
            result.Y[i] = std::min(static_cast<int>(Y[i]) + rhs.Y[i], 255);

        for (size_t i = 0; i < U.size(); ++i)
            result.U[i] = std::min(static_cast<int>(U[i]) + rhs.U[i], 255);

        for (size_t i = 0; i < V.size(); ++i)
            result.V[i] = std::min(static_cast<int>(V[i]) + rhs.V[i], 255);

        return result;
    }
};


#pragma pack(push, 1)
struct BITMAPFILEHEADER {
    uint16_t bfType;      
    uint32_t bfSize;      
    uint16_t bfReserved1; 
    uint16_t bfReserved2;
    uint32_t bfOffBits;   
};


struct BITMAPINFOHEADER {
    uint32_t biSize;          
    int32_t  biWidth;         
    int32_t  biHeight;        
    uint16_t biPlanes;        
    uint16_t biBitCount;      
    uint32_t biCompression;   
    uint32_t biSizeImage;     
    int32_t  biXPelsPerMeter; 
    int32_t  biYPelsPerMeter; 
    uint32_t biClrUsed;       
    uint32_t biClrImportant;  
};
#pragma pack(pop)


struct BmpFile
{
    std::ifstream file;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    BmpFile(std::ifstream& file, BITMAPFILEHEADER bitmapFileHeader, BITMAPINFOHEADER bitmapInfoHeader)
        : file(std::move(file)), fileHeader(bitmapFileHeader), infoHeader(bitmapInfoHeader) {}
};

struct Yuv420File
{
    std::string path;     
    size_t width = 0;        
    size_t height = 0;       
    size_t frameSize = 0;
    
    Yuv420File(const std::string& filePath, size_t width, size_t height)
        : path(filePath), width(width), height(height)
    {
        frameSize = width * height + 2 * (width / 2) * (height / 2); // Y + U + V
    }
};