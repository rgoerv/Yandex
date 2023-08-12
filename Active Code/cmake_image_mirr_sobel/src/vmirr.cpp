#include "ppm_image.h"

#include <algorithm>
#include <iostream>
#include <string_view>

using namespace std;

void VMirrInplace(img_lib::Image& image) {

    auto h = image.GetHeight();
    auto w = image.GetWidth();
    auto mid = (h / 2) % 2 == 0 ? h / 2 : (h / 2) - 1;

    for(auto y = 0; y < mid; ++y) {
        std::swap_ranges(image.GetLine(y), image.GetLine(y) + w, image.GetLine(h - y - 1));
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