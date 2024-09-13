// BMPs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

const int WIDTH = 1000;
const int HEIGHT = 100;

const int PIXEL_BYTES = 3; // RGB
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

#pragma pack(push,1)
struct StockBitmapHeader { 
    // File Header
    uint16_t identifier;
    uint32_t fileSize;
    uint32_t resevedSpace;
    uint32_t offset;

    // Info Header
    uint32_t infoHeaderSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint32_t compression;
    uint16_t bitsPerPixel;
    
    uint32_t imageSize;
    uint32_t XPixelsPerMeter;
    uint32_t YPixelsPerMeter;
    uint32_t usedColours;
    uint32_t importantColours;

    StockBitmapHeader(unsigned int Width, unsigned int Height ) {
        // File Header
        identifier = 'MB';
        fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (Width * Height);
        resevedSpace = 0;
        offset = FILE_HEADER_SIZE + INFO_HEADER_SIZE;
        // Info Header - Used
        infoHeaderSize = INFO_HEADER_SIZE;
        width = Width;
        height = Height;
        planes = 1;
        compression = PIXEL_BYTES * 8;
        // Ifno Header - Unused
        bitsPerPixel = 0;
        imageSize = 0;
        XPixelsPerMeter = 0;
        YPixelsPerMeter = 0;
        usedColours = 0;
        importantColours = 0;
    }

};
#pragma pack(pop)

void generateBitmap(unsigned char* imageData, unsigned int width, unsigned int height, char* fileName)
    {
    
    FILE* bmpFile = fopen(fileName, "wb");
    StockBitmapHeader* header = new StockBitmapHeader(width, height);
    fwrite
    (
        reinterpret_cast<unsigned char*>(header),
        1, 
        FILE_HEADER_SIZE + INFO_HEADER_SIZE, 
        bmpFile
    );
    delete(header);
    for (unsigned int currentHeight = 0; currentHeight < height; currentHeight++) {
        fwrite((imageData + (currentHeight * width * PIXEL_BYTES)), PIXEL_BYTES, width, bmpFile);
    }

    fclose(bmpFile);
}

int main()
{

    unsigned char* image = reinterpret_cast<unsigned char*>(malloc(sizeof(unsigned char) * WIDTH * HEIGHT * PIXEL_BYTES));

    char* bmpFileName = const_cast<char*>("bitmap.bmp");
    for (unsigned int currentWidth = 0; currentWidth < WIDTH; currentWidth++) {
        for (unsigned int currentHeight = 0; currentHeight < HEIGHT; currentHeight++) {
            
            int index = currentWidth * (HEIGHT * PIXEL_BYTES) + currentHeight * PIXEL_BYTES;
            
            image[index + 0] = static_cast<unsigned char>(255);
            image[index + 1] = static_cast<unsigned char>(255);
            image[index + 2] = static_cast<unsigned char>(0);

        }
        
    }

    generateBitmap(image, WIDTH, HEIGHT, bmpFileName);

    return 0;
}

