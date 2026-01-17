#include <iostream>
#include <fstream>

#pragma pack(push, 1) // Выравнивание структуры
struct BMPFileHeader {
    short bfType=0x4D42; // "BM"
    unsigned int bfSize=0;      // Размер файла в байтах
    short bfReserved1=0;
    short bfReserved2=0;
    unsigned int bfOffBits=20+54;  // Смещение до начала данных
};

struct BMPInfoHeader {
    unsigned int biSize=40;      // Размер структуры
    int biWidth=0;       // Ширина изображения
    int biHeight=0;      // Высота изображения
    short biPlanes=1;     // Количество цветовых плоскостей
    short biBitCount=4;  // Биты на пиксель
    unsigned int biCompression=0; // Без сжатия
    unsigned int biSizeImage=0;  // Размер изображения (можно 0)
    int biXPelsPerMeter=0; // Горизонтальное разрешение
    int biYPelsPerMeter=0; // Вертикальное разрешение
    unsigned int biClrUsed=5;     // Количество используемых цветов
    unsigned int biClrImportant=0; // Количество важных цветов
};


uint8_t ColorTable[20] = {
    255, 255, 255, 0,
    0, 255, 0, 0,
    255, 0, 255, 0,
    0, 255, 255, 0,
    0, 0, 0, 0
};
#pragma pack(pop)

void createBMP(const char* filename, int width, int height, long long** matrix, int upper_border, int left_border) {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    
    int byte_row = (infoHeader.biWidth+1)/2;
    byte_row = ((byte_row + 3) / 4) * 4;
    infoHeader.biSizeImage=byte_row * infoHeader.biHeight;
    fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

    std::ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка при открытии файла!" << std::endl;
        return;
    }

    // Записываем заголовки
    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    file.write(reinterpret_cast<const char*>(&ColorTable), sizeof(ColorTable));
    // Записываем данные пикселей
    for (int i = upper_border; i < height+upper_border; ++i) {
        for (int j = left_border; j < width+left_border; j += 2) {
            uint8_t byte = matrix[i][j] << 4;
            if (j + 1 < width+left_border) {
                byte |= matrix[i][j + 1];
            }
            file.write(reinterpret_cast<const char*>(&byte), 1);
        }
        uint8_t padding = 0;

        for (int j = 0; j < byte_row - ((infoHeader.biWidth + 1) / 2); ++j) {
            file.write(reinterpret_cast<const char*>(&padding), 1);
        }
    }

    file.close();
    std::cout << "BMP файл успешно создан!" << "\n";
}