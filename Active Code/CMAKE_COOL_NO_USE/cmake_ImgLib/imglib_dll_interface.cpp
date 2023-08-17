#include "img_lib.h"
#include "ppm_image.h"
#include "imglib_dll_interface.h"

#include <cassert>

struct ImgLibImage {
    img_lib::Image img;
};

ImgLibImage* ImgLibImgCreate(int width, int height, ImgLibColor fill) {
    img_lib::Image image(width, height, reinterpret_cast<img_lib::Color&>(fill));
    return new ImgLibImage{std::move(image)};
}

void ImgLibImgFree(ImgLibImage* img) {
    delete img;
}

int ImgLibImgGetWidth(const ImgLibImage* img) {
    assert(img);
    return img->img.GetWidth();
}

int ImgLibImgGetHeight(const ImgLibImage* img) {
    assert(img);
    return img->img.GetHeight();
}

int ImgLibImgGetStep(const ImgLibImage* img) {
    assert(img);
    return img->img.GetStep();
}

bool ImgLibImgIsInited(const ImgLibImage* img) {
    return img && img->img;
}

ImgLibColor ImgLibImgGetPixelConst(const ImgLibImage* img, int x, int y) {
    assert(img);
    img_lib::Color val = img->img.GetPixel(x, y);
    ImgLibColor& dll_val = reinterpret_cast<ImgLibColor&>(val);
    return dll_val;
}

ImgLibColor& ImgLibImgGetPixel(ImgLibImage* img, int x, int y) {
    assert(img);
    return reinterpret_cast<ImgLibColor&>(img->img.GetPixel(x, y));
}

const ImgLibColor* ImgLibImgGetLineConst(const ImgLibImage* img, int line) {
    assert(img);
    return reinterpret_cast<const ImgLibColor*>(img->img.GetLine(line));
}

ImgLibColor* ImgLibImgGetLine(ImgLibImage* img, int line) {
    assert(img);
    return reinterpret_cast<ImgLibColor*>(img->img.GetLine(line));
}

ImgLibImage* ImgLibLoadPPM(const char* path) {
    img_lib::Image loaded = img_lib::LoadPPM(path);
    if (!loaded) {
        return nullptr;
    }

    return new ImgLibImage{std::move(loaded)};
}

bool ImgLibSavePPM(const char* path, const ImgLibImage* image) {
    assert(image);
    return img_lib::SavePPM(path, image->img);
}