// файл библиотеки включаем через угловые скобки, прямо как файлы stdlib
#include <ppm_image.h>

#include <algorithm>
#include <numeric>
#include <iostream>
#include <string_view>
#include <random>

using namespace std;

// Свой велосипед для получения равномерно распределённого числа от 0 до max - 1.
// В отличие от uniform_int_distribution его результат будет одинаковым на всех платформах
template <typename Engine>
uint32_t GetRandomNumber(uint32_t max, Engine& engine) {
    uint32_t engine_max = static_cast<uint32_t>(engine.max());

    if (max == engine_max) {
        return static_cast<uint32_t>(engine());
    }

    while (true) {
        uint32_t val = static_cast<uint32_t>(engine());

        if (val / max < engine_max / max || engine_max % max == max - 1u) {
            return val % max;
        }
    }
}

// У нас будет свой Shuffle. В отличие от std::shuffle он будет работать одинаково везде
template <typename RandomIt, typename Engine>
void Shuffle(RandomIt first, RandomIt last, Engine& engine) {
    using IterDiff = typename std::iterator_traits<RandomIt>::difference_type;
    IterDiff i, n;
    n = last - first;
    for (i = n - 1; i > 0; --i) {
        swap(first[i], first[GetRandomNumber(static_cast<uint32_t>(i + 1), engine)]);
    }
}

void CypherInplace(unsigned int seed, img_lib::Image& image) {
    mt19937 generator(seed);
    const int area = image.GetWidth() * image.GetHeight();

    // координаты пикселя по индексу
    auto px_num_to_coordinates = [&image](int num) {
        return std::pair{num % image.GetWidth(), num / image.GetWidth()};
    };

    // сложим индексы половины пикселей в вектор и перемешаем их
    std::vector<int> positions(area / 2);
    std::iota(positions.begin(), positions.end(), 0);
    Shuffle(positions.begin(), positions.end(), generator);

    // Будем переставлять чётные и нечётные пиксели между собой.
    // Благодаря этому процедура шифрования и дешифрования будет одинаковой.
    for (int px_num = 0; px_num < area/2; ++px_num) {
        auto [src_x, src_y] = px_num_to_coordinates(px_num * 2);
        auto [dst_x, dst_y] = px_num_to_coordinates(positions[px_num] * 2 + 1);

        std::swap(image.GetPixel(src_x, src_y), image.GetPixel(dst_x, dst_y));
    }
}

int main(int argc, const char** argv) {
    if (argc != 4) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image> <key>"sv << endl;
        return 1;
    }

    int key = atoi(argv[3]);

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    CypherInplace(key, image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}