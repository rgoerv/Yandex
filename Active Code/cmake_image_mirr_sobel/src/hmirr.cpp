#include "ppm_image.h"

#include <iostream>
#include <string_view>
#include <algorithm>

using namespace std;

void HMirrInplace(img_lib::Image& image){
    auto h = image.GetHeight();
    auto w = image.GetWidth();
    auto mid = w / 2;

    for(auto y = 0; y < h; ++y) {
        for(auto x = 0; x < mid; ++x) {
            swap(image.GetPixel(x, y).r, image.GetPixel(w - x - 1, y).r);
            swap(image.GetPixel(x, y).g, image.GetPixel(w - x - 1, y).g);
            swap(image.GetPixel(x, y).b, image.GetPixel(w - x - 1, y).b);
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

    HMirrInplace(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}