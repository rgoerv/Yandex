#pragma once
#include "img_lib.h"

#include <filesystem>

namespace img_lib {
using Path = std::filesystem::path;

Image LoadJPEG(const Path& file);

bool SaveJPEG(const Path& file, const Image& image);

} // of namespace img_lib