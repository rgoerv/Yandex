#pragma once
#include <cstddef>

// в этом файле определён макрос IMGLIB_EXPORT
#include "imglib_dll_defines.h"

// Даже в динамической библиотеке можно определять простые структуры.
// Так как имя функции должно быть простым идентификатором,
// namespace использовать нельзя. Поэтому все имена снабдим 
// префиксом ImgLib
struct ImgLibSize {
    int width;
    int height;
};

struct ImgLibColor {
    // inline-метод также допустим, но вынести определение в cpp-файл
    // не получится
    static ImgLibColor Black() {
        return {std::byte{0}, std::byte{0}, std::byte{0}, std::byte{255}};
    }
    std::byte r, g, b, a;
};

// Определения этого класса в интерфейсе не будет.
// Но указатель на него тем не менее использовать можно.
class ImgLibImage;

// Так теперь выглядит "конструктор" изображения.
// Любое сконструированное изображение необходимо 
// будет удалить функцией ImgLibImgFree
IMGLIB_EXPORT ImgLibImage* ImgLibImgCreate(int width, int height, ImgLibColor fill);
IMGLIB_EXPORT void ImgLibImgFree(ImgLibImage* img);

// Перед всеми экспортируемыми функциями явно пишем IMGLIB_EXPORT.
// Когда будем включать этот файл при использованиии библиотеки, макрос 
// будет определён по-другому
IMGLIB_EXPORT int ImgLibImgGetWidth(const ImgLibImage* img);
IMGLIB_EXPORT int ImgLibImgGetHeight(const ImgLibImage* img);
IMGLIB_EXPORT int ImgLibImgGetStep(const ImgLibImage* img);
IMGLIB_EXPORT bool ImgLibImgIsInited(const ImgLibImage* img);
IMGLIB_EXPORT ImgLibColor ImgLibImgGetPixelConst(const ImgLibImage* img, int x, int y);
IMGLIB_EXPORT ImgLibColor& ImgLibImgGetPixel(ImgLibImage* img, int x, int y);
IMGLIB_EXPORT const ImgLibColor* ImgLibImgGetLineConst(const ImgLibImage* img, int line);
IMGLIB_EXPORT ImgLibColor* ImgLibImgGetLine(ImgLibImage* img, int line);

IMGLIB_EXPORT ImgLibImage* ImgLibLoadPPM(const char* path);

// inline-функция может быть определена явно,
// она не будет экспортирована.
inline void ImgLibUnloadPPM(ImgLibImage* image) {
    ImgLibImgFree(image);
}

// Здесь мы использовали const char* вместо std::filesystem::path,
// поскольку классы стандартной библиотеки в интерфейсе библиотеки нежелательны -
// они могут различаться при компиляции библиотеки и программы, её
// использующей.
IMGLIB_EXPORT bool ImgLibSavePPM(const char* path, const ImgLibImage* image);