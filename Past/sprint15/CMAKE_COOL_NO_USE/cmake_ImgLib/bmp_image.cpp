#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

#include <iostream>


using namespace std;

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapFileHeader {
    char sign1 = 'B';
    char sign2 = 'M';
    uint32_t size_header_data;
    uint32_t reserve = 0;
    uint32_t data_padding = 54;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    int32_t header = 40;
    int32_t width;
    int32_t height;
    uint16_t plane_count = 1;
    uint16_t BPP = 24; // Bit Per Pixel
    uint32_t compress_type = 0;
    uint32_t bit_count_in_data;
    int32_t h_resolution = 11811;
    int32_t v_resolution = 11811;
    int32_t color_use_count = 0;
    int32_t important_color_count = 0x1000000;
}
PACKED_STRUCT_END

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    ofstream out(file, ios::binary);

    const auto& h = image.GetHeight();
    const auto& w = image.GetWidth();

    const auto& stride = GetBMPStride(w);

    BitmapFileHeader file_header;
    file_header.size_header_data = file_header.data_padding + (stride * h);

    out.write(reinterpret_cast<char*>(&file_header), sizeof(decltype(file_header)));

    BitmapInfoHeader info_header;
    info_header.width = w;
    info_header.height = h;
    info_header.bit_count_in_data = stride * h;
    
    out.write(reinterpret_cast<char*>(&info_header), sizeof(decltype(info_header)));
    
    std::vector<char> buff(stride);

    for (int y = info_header.height - 1; y >= 0; --y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < w; ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        out.write(buff.data(), stride);
    }

    return out.good();
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    ifstream ifs(file, ios::binary);

    BitmapFileHeader file_header;
    ifs.read(reinterpret_cast<char*>(&file_header), sizeof(BitmapFileHeader));

    BitmapInfoHeader info_header;
    ifs.read(reinterpret_cast<char*>(&info_header), sizeof(BitmapInfoHeader));

    if (BitmapInfoHeader check; 
        file_header.sign1 != BitmapFileHeader().sign1 || 
        file_header.sign2 != BitmapFileHeader().sign2 ||
        info_header.plane_count != check.plane_count ||
        info_header.BPP != check.BPP ||
        info_header.compress_type != check.compress_type ||
        info_header.h_resolution != check.h_resolution ||
        info_header.v_resolution != check.v_resolution ||
        info_header.color_use_count != check.color_use_count ||
        info_header.important_color_count != check.important_color_count) 
    {
        return {};
    }

    const auto& stride = GetBMPStride(info_header.width);

    std::vector<char> buff(stride);
    Image result(info_header.width, info_header.height, Color::Black());

    for (int y = info_header.height - 1; y >= 0; --y) {
        ifs.read(buff.data(), stride);
        Color* line = result.GetLine(y);

        for (int x = 0; x < info_header.width; ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }

    return result;
}

}  // namespace img_lib