#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;


/*
Пример использования библиотеки. Он будет компилироваться и работать, когда вы реализуете
все классы библиотеки.
*/

namespace {

Polyline CreateStar(Point center, double outer_rad, double inner_rad, int num_rays)
{
    Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
        double angle = 2 * M_PI * (i % num_rays) / num_rays;
        polyline.AddPoint({ center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle) });
        if (i == num_rays) {
            break;
        }
        angle += M_PI / num_rays;
        polyline.AddPoint({ center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle) });
    }
    return polyline;
}

}

namespace shapes {

class Triangle : public svg::Drawable {
public:
    Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
        : p1_(p1)
        , p2_(p2)
        , p3_(p3) {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer& container) const override {
        container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
    }

private:
    svg::Point p1_, p2_, p3_;
};

class Star : public svg::Drawable {
public:
    Star(Point center, double outer_rad, double inner_rad, int num_rays)
        : center_(center)
        , outer_rad_(outer_rad)
        , inner_rad_(inner_rad)
        , num_rays_(num_rays)
    {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer& container) const override {
        container.Add(CreateStar(center_, outer_rad_, inner_rad_, num_rays_).SetFillColor("red"s).SetStrokeColor("black"s));
    }

private:
    svg::Point center_;
    double outer_rad_;
    double inner_rad_;
    int num_rays_;
};

class Snowman : public svg::Drawable {
public:
    Snowman(svg::Point center, double radius)
        : center_(center)
        , raduis_(radius)
        {
        }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer& container) const override {
        container.Add(svg::Circle()
            .SetCenter({ center_.x, center_.y + (5.0 * raduis_) })
            .SetRadius(2.0 * raduis_)
            .SetFillColor("rgb(240,240,240)"s)
            .SetStrokeColor("black"s));
        container.Add(svg::Circle()
            .SetCenter({ center_.x, center_.y + (2.0 * raduis_) })
            .SetRadius(1.5 * raduis_).SetFillColor("rgb(240,240,240)"s)
            .SetStrokeColor("black"s));
        container.Add(svg::Circle()
            .SetCenter(center_)
            .SetRadius(raduis_)
            .SetFillColor("rgb(240,240,240)"s)
            .SetStrokeColor("black"s));
    }

private:
    svg::Point center_;
    double raduis_;
};

} // namespace shapes


int main() {
    using namespace svg;
    using namespace std;

    Color none_color;
    cout << none_color << endl; // none
    
    Color purple{"purple"s};
    cout << purple << endl; // purple
    
    Color rgb = Rgb{100, 200, 255};
    cout << rgb << endl; // rgb(100,200,255)

    Color rgba = Rgba{100, 200, 255, 0.5};
    cout << rgba << endl; // rgba(100,200,255,0.5)

    Circle c;
    c.SetRadius(3.5).SetCenter({1.0, 2.0});
    c.SetFillColor(rgba);
    c.SetStrokeColor(purple);
    
    Document doc;
    doc.Add(std::move(c));
    doc.Render(cout);
    system("pause");
} 
// > "C:\Programs MVSC\libsvg\x64\Debug\outsvg.svg"