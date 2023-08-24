#include "ppm_image.h"
#include "img_lib.h"

#include <algorithm>
#include <iostream>
#include <string_view>

using namespace std;

void VMirrInplace(img_lib::Image& image) {

    auto h = image.GetHeight();
    auto w = image.GetWidth();
    auto mid = h / 2;

    for(auto y = 0; y < mid; ++y) {
        for(auto x = 0; x < w; ++x) {
            swap(image.GetPixel(x, y).r, image.GetPixel(x, h - y - 1).r);
            swap(image.GetPixel(x, y).g, image.GetPixel(x, h - y - 1).g);
            swap(image.GetPixel(x, y).b, image.GetPixel(x, h - y - 1).b);
        }
    }
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

    VMirrInplace(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}