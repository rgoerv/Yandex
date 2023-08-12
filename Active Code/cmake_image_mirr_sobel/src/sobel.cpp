#include "ppm_image.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string_view>

using namespace std;

int Sum(img_lib::Color c) {
    return to_integer<int>(c.r) + to_integer<int>(c.g) + to_integer<int>(c.b);
}

// реализуйте оператор Собеля
img_lib::Image Sobel(const img_lib::Image& image) {

    int h = image.GetHeight();
    int w = image.GetWidth();
    
    img_lib::Image monochrome(w, h, img_lib::Color::Black());

    for(int y = 1; y < h - 1; ++y) {

        img_lib::Color* line = monochrome.GetLine(y);

        int bl, bc, br, cl, cr, tl, tc, tr;
        int gx, gy;
        double gs; // general result operator Sobel 

        for(int x = 1; x < w - 1; ++x) {

            bl = Sum(image.GetPixel(x - 1, y - 1));
            bc = Sum(image.GetPixel(x, y - 1));
            br = Sum(image.GetPixel(x + 1, y - 1));

            cl = Sum(image.GetPixel(x - 1, y));
            cr = Sum(image.GetPixel(x + 1, y));

            tl = Sum(image.GetPixel(x - 1, y + 1));
            tc = Sum(image.GetPixel(x, y + 1));
            tr = Sum(image.GetPixel(x + 1, y + 1));

            gx = -tl - 2*tc - tr + bl +2*bc + br;
            gy = -tl - 2*cl - bl + tr + 2*cr + br;

            gs = std::sqrt(std::pow(gx, 2) + std::pow(gy, 2));
            
            auto monochrome_pixel = static_cast<std::byte>(std::clamp<double>(gs, 0, 255));

            line[x].r = monochrome_pixel;
            line[x].g = monochrome_pixel;
            line[x].b = monochrome_pixel;
        }
    }

    return monochrome;
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    image = Sobel(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}