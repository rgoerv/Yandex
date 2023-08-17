#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

enum class Format {
    JPEG,
    PPM,
    UNKNOWN
};

Format GetFormatByExtension(const img_lib::Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return Format::PPM;
    }

    return Format::UNKNOWN;
}

class ImageFormatInterface {
public:
    virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
    virtual img_lib::Image LoadImage(const img_lib::Path& file) const = 0;
};

class FormatJPEG : public ImageFormatInterface {
public:
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
        return img_lib::SaveJPEG(file, image);
    }
    img_lib::Image LoadImage(const img_lib::Path& file) const override {
        return img_lib::LoadJPEG(file);
    }
};

class FormatPPM : public ImageFormatInterface {
public:
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
        return img_lib::SavePPM(file, image);
    }
    img_lib::Image LoadImage(const img_lib::Path& file) const override {
        return img_lib::LoadPPM(file);
    }
};

ImageFormatInterface* GetFormatInterface(const img_lib::Path& path) {
    Format file_format = GetFormatByExtension(path);
    static const FormatJPEG jpeg;
    static const FormatPPM ppm;

    switch (file_format)
    {
        case Format::JPEG: return (ImageFormatInterface*)&jpeg; break;
        case Format::PPM: return (ImageFormatInterface*)&ppm; break;
        case Format::UNKNOWN: return nullptr; break;
    default:
        break;
    }
    return nullptr;
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];

    ImageFormatInterface* in_format_intf = GetFormatInterface(in_path);

    if(!in_format_intf) {
        cerr << "Loading failed"sv << endl;
        return 3;
    }

    ImageFormatInterface* out_format_intf = GetFormatInterface(out_path);

    if(!out_format_intf) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }

    img_lib::Image image = in_format_intf->LoadImage(in_path);
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return 5;
    }

    if (!out_format_intf->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 6;
    }

    cout << "Successfully converted"sv << endl;
}