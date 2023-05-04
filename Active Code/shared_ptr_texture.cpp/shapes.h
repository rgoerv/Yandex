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
        ShapeType type;
        if (type_ == ShapeType::RECTANGLE)
        {
            DrawRectangle(image);
        }
    }

    void DrawRectangle(Image& image) const
    {
        
    }

    void DrawEllipse(Image& image) const
    {
        
    }

private:
    ShapeType type_;
    Size size_;
    Point point_;
    std::shared_ptr<Texture> texture_;
};