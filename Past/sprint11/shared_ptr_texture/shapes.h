#pragma once
#include "texture.h"

#include <memory>
#include <iostream>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE};

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type) 
        : type_(type)
        , size_(Size{})
        , point_(Point{})
        , texture_(nullptr)
        {    
        }

    void SetPosition(Point pos) {
        point_ = pos;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_.reset();
        texture_.swap(texture);
    }

    // Рисует фигуру на указанном изображении
	// В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        if (type_ == ShapeType::RECTANGLE)
        {
            DrawRectangle(image);
        }
        else
        {
            DrawEllipse(image);
        }
    }

    void DrawRectangle(Image& image) const
    {
        Size image_size = GetImageSize(image);

        Size texture_size { 0, 0 };
        if (texture_) {
            texture_size = texture_->GetSize();
        }

        int th = texture_size.height;
        int tw = texture_size.width;

        int px = point_.x;
        int py = point_.y;

        if (point_.x < 0) {
            tw -= point_.x;
            px = 0;
        }
        if (point_.y < 0) {
            th -= point_.y;
            py = 0;
        }

        for (int y = 0; y < image_size.height; ++y) {
            for (int x = 0; x < image_size.width; ++x) {
                
                if (y + py >= image_size.height) {
                    continue;
                }

                if (x >= size_.width || y >= size_.height) {
                    continue;
                }

                if (x + px >= image_size.width) {
                    continue;
                }

                if (x >= tw || y >= th) {
                    image[y + py][x + px] = '.';
                } else {
                    image[y + py][x + px] = texture_->GetPixelColor({ y, x });
                }
            }
        }
    }


    void DrawEllipse(Image& image) const
    {
        Size image_size = GetImageSize(image);

        Size texture_size { 0, 0 };
        if (texture_) {
            texture_size = texture_->GetSize();
        }

        int th = texture_size.height;
        int tw = texture_size.width;

        int px = point_.x;
        int py = point_.y;

        if (point_.x < 0) {
            tw -= point_.x;
            px = 0;
        }
        if (point_.y < 0) {
            th -= point_.y;
            py = 0;
        }

        for (int y = 0; y < image_size.height; ++y) {
            
            if (y + py >= image_size.height) {
                continue;
            }

            for (int x = 0; x < image_size.width; ++x) {
                if (x >= size_.width || y >= size_.height) {
                    continue;
                }

                if (x + px >= image_size.width) {
                    continue;
                }

                if (x >= tw || y >= th) {
                    if (IsPointInEllipse({ x, y }, size_)) {
                        image[y + py][x + px] = '.';
                    }
                } else {
                    if (IsPointInEllipse({ x, y }, size_)) {
                        image[y + py][x + px] = texture_->GetPixelColor({ y, x });
                    }
                }
            }
        }
    }

private:
    ShapeType type_;
    Size size_;
    Point point_;
    std::shared_ptr<Texture> texture_;
};