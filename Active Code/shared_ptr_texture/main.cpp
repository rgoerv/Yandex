#include "canvas.h"
#include "shapes.h"

#include <cassert>
#include <sstream>

std::unique_ptr<Texture> MakeTextureCow() {
    Image image = {R"(^__^            )",  //
                   R"((oo)\_______    )",  //
                   R"((__)\       )\/\)",  //
                   R"(    ||----w |   )",  //
                   R"(    ||     ||   )"};
    return std::make_unique<Texture>(move(image));
}

std::unique_ptr<Texture> MakeTextureSolid(Size size, char pixel) {
    Image image(size.height, std::string(size.width, pixel));
    return std::make_unique<Texture>(move(image));
}

std::unique_ptr<Texture> MakeTextureCheckers(Size size, char pixel1, char pixel2) {
    Image image(size.height, std::string(size.width, pixel1));

    for (int i = 0; i < size.height; ++i) {
        for (int j = 0; j < size.width; ++j) {
            if ((i + j) % 2 != 0) {
                image[i][j] = pixel2;
            }
        }
    }

    return std::make_unique<Texture>(move(image));
}

void TestCpp() {
    Canvas canvas(Size{77, 17});

    // Буква "C" как разность двух эллипсов, один из которых нарисован цветом фона
    canvas.AddShape(ShapeType::ELLIPSE, {2, 1}, {30, 15},
                    MakeTextureCheckers({100, 100}, 'c', 'C'));
    canvas.AddShape(ShapeType::ELLIPSE, {8, 4}, {30, 9}, MakeTextureSolid({100, 100}, ' '));

    // Горизонтальные чёрточки плюсов
    auto h1 = canvas.AddShape(ShapeType::RECTANGLE, {54, 7}, {22, 3},
                              MakeTextureSolid({100, 100}, '+'));
    canvas.DuplicateShape(h1, {30, 7});

    // Вертикальные чёрточки плюсов
    auto v1 = canvas.DuplicateShape(h1, {62, 3});
    canvas.ResizeShape(v1, {6, 11});
    canvas.DuplicateShape(v1, {38, 3});

    std::stringstream output;
    canvas.Print(output);

    const auto answer
        = "###############################################################################\n"
          "#                                                                             #\n"
          "#            cCcCcCcCcC                                                       #\n"
          "#        CcCcCcCcCcCcCcCcCc                                                   #\n"
          "#      cCcCcCcCcCcCcCcCcCcCcC          ++++++                  ++++++         #\n"
          "#    CcCcCcCcCcCc                      ++++++                  ++++++         #\n"
          "#   CcCcCcCcC                          ++++++                  ++++++         #\n"
          "#   cCcCcCc                            ++++++                  ++++++         #\n"
          "#  cCcCcC                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
          "#  CcCcCc                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
          "#  cCcCcC                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
          "#   cCcCcCc                            ++++++                  ++++++         #\n"
          "#   CcCcCcCcC                          ++++++                  ++++++         #\n"
          "#    CcCcCcCcCcCc                      ++++++                  ++++++         #\n"
          "#      cCcCcCcCcCcCcCcCcCcCcC          ++++++                  ++++++         #\n"
          "#        CcCcCcCcCcCcCcCcCc                                                   #\n"
          "#            cCcCcCcCcC                                                       #\n"
          "#                                                                             #\n"
          "###############################################################################\n";

    assert(answer == output.str());
}

void TestCow() {
    Canvas canvas{{18, 5}};

    canvas.AddShape(ShapeType::RECTANGLE, {1, 0}, {16, 5}, MakeTextureCow());

    std::stringstream output;
    canvas.Print(output);

    // clang-format off
    // Здесь уместно использовать сырые литералы, т.к. в текстуре есть символы '\'
    const auto answer =
        R"(####################)""\n"
        R"(# ^__^             #)""\n"
        R"(# (oo)\_______     #)""\n"
        R"(# (__)\       )\/\ #)""\n"
        R"(#     ||----w |    #)""\n"
        R"(#     ||     ||    #)""\n"
        R"(####################)""\n";
    // clang-format on

    assert(answer == output.str());
}

void BigDraw()
{
    Canvas canvas({ 3000, 2000 });

    canvas.AddShape(ShapeType::RECTANGLE, { 100, 100 }, { 4000, 20 },
        MakeTextureSolid({ 3, 1000 }, '*'));

    canvas.AddShape(ShapeType::ELLIPSE, { 50, 50 }, { 30, 900 },
        MakeTextureSolid({ 100, 100 }, '&'));

    std::stringstream output;
    canvas.Print(output);

    std::string answer;
    answer.reserve(3002 * 2003);

    for (int i = 0; i < 3002; ++i) {
        answer.push_back('#');
    }
    answer.push_back('\n');

    for (int i = 0; i < 2000; ++i) {
        answer.push_back('#');
        for (int j = 0; j < 3000; ++j) {
            if (IsPointInEllipse({ j - 50, i - 50 }, { 30, 900 })) {
                if (i < 150) {
                    answer.push_back('&');
                } else {
                    answer.push_back('.');
                }
            } else if (i >= 100 && j >= 100 && j < 4100 && i < 120) {
                if (j < 103) {
                    answer.push_back('*');
                } else {
                    answer.push_back('.');
                }
            } else {
                answer.push_back(' ');
            }
        }
        answer.push_back('#');
        answer.push_back('\n');
    }

    for (int i = 0; i < 3002; ++i) {
        answer.push_back('#');
    }
    answer.push_back('\n');

    assert(answer == output.str());
}


int main() {
    BigDraw();
    TestCow();
    TestCpp();

    Canvas canvas({ 30, 20 });

    canvas.AddShape(ShapeType::RECTANGLE, { 10, 10 }, { 40, 2 },
        MakeTextureSolid({ 3, 1000 }, '*'));

    canvas.AddShape(ShapeType::ELLIPSE, { 5, 5 }, { 3, 90 },
        MakeTextureSolid({ 100, 100 }, '&'));
    
    canvas.Print(std::cout);

    system("pause");
}