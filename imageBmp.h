#pragma once
#include <fstream>
#include "dataContainers.h"


class ImageBmp
{
public:
    ImageBmp() = default;
    
    void setNewImageFromFile(const std::string& fileName);
    const MatrixRGB& getPixelsMatrix() const;
    
private:
    
    MatrixRGB pixel_matrix;

    BmpFile open_file(const std::string& pathToFile) const;
    MatrixRGB get_bmp_pixel_matrix(BmpFile& bmpFile) const;
};
