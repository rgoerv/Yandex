// файл нашей библиотеки включаем через угловые скобки, прямо как файлы stdlib
#include <imglib_dll_interface.h>

#include <algorithm>
#include <iostream>
#include <string_view>

using namespace std;

byte Multiply(byte component, double multiplier) {
    const double res = static_cast<double>(component) * multiplier;
    return static_cast<byte>(clamp(res, 0., 255.));
}

ImgLibColor Multiply(ImgLibColor color, double multiplier) {
    return {Multiply(color.r, multiplier), Multiply(color.g, multiplier), Multiply(color.b, multiplier), color.a};
}

int main(int argc, const char** argv) {
    if (argc != 4) {
        cerr << "Expected 3 arguments: input name, output name, luma multiplier"sv << endl;
        return 1;
    }

    double multiplier = atof(argv[3]);

    ImgLibImage* image = ImgLibLoadPPM(argv[1]);

    // Проверку указателя на равенство nullptr
    // можно выполнять операцией !
    if (!image) {
        cerr << "Incorrect PPM image file"sv << endl;
        return 2;
    }

    for (int y = 0; y < ImgLibImgGetHeight(image); ++y) {
        auto line = ImgLibImgGetLine(image, y);
        for (int x = 0; x < ImgLibImgGetWidth(image); ++x) {
             line[x] = Multiply(line[x], multiplier);
        }
    }

    if (!ImgLibSavePPM(argv[2], image)) {
        cerr << "Saving PPM image failed"sv << endl;
        ImgLibUnloadPPM(image);
        return 3;
    }

    // Из-за примитивного интерфейса библиотеки выгружать изображение приходится
    // вручную, причём при каждом выходе из функции. Можно написать обёртку
    // над обёрткой, но в такой маленькой программе обойдёмся без неё
    ImgLibUnloadPPM(image);
}