#include "svg.h"
#include <iostream>
#include <iomanip>

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

void OstreamColorPrinter::operator()(std::monostate) const {
    out << "none";
}

void OstreamColorPrinter::operator()(std::string str) const {
    out << str;
}

void OstreamColorPrinter::operator()(svg::Rgb rgb) const {
    out << "rgb("s << std::to_string(rgb.red) << ","s << std::to_string(rgb.green) << ","s << std::to_string(rgb.blue) << ")"s;
}

void OstreamColorPrinter::operator()(svg::Rgba rgba) const {
    out << "rgba("s << std::to_string(rgba.red) << ","s << std::to_string(rgba.green) << ","s << std::to_string(rgba.blue) << ","s << rgba.opacity << ")"s;
}

std::ostream& operator<<(std::ostream& out, const Color& color) {
    using namespace std::string_view_literals;
    
    std::visit(svg::OstreamColorPrinter { out }, color);

    return out;
}

std::ostream& operator<<(std::ostream& out, const StrokeLineCap& line_cap) {
    using namespace std::string_view_literals;
    switch (line_cap) {
    case StrokeLineCap::BUTT: {
        out << "butt"sv;
        break;
    }
    case StrokeLineCap::ROUND: {
        out << "round"sv;
        break;
    }
    case StrokeLineCap::SQUARE: {
        out << "square"sv;
        break;
        [[fallthrough]];
    }
    default:
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const StrokeLineJoin& line_join) {
    using namespace std::string_view_literals;
    switch (line_join) {
    case StrokeLineJoin::ARCS: {
        out << "arcs"sv;
        break;
    }
    case StrokeLineJoin::BEVEL: {
        out << "bevel"sv;
        break;
    }
    case StrokeLineJoin::MITER: {
        out << "miter"sv;
        break;
    }
    case StrokeLineJoin::MITER_CLIP: {
        out << "miter-clip"sv;
        break;
    }
    case StrokeLineJoin::ROUND: {
        out << "round"sv;
        break;
        [[fallthrough]];
    }
    default:
        break;
    }
    return out;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\""sv;
    RenderAttrs(context.out);
    out << "/>"sv;
}

// ---------- Polyline ----------------

// Добавляет очередную вершину к ломаной линии
Polyline& Polyline::AddPoint(Point point) {
    points.push_back(point);
    return *this;
}
void Polyline::RenderObject(const RenderContext& context) const {    
    auto& out = context.out;
    out << "<polyline points=\""sv;

    bool is_first = true;
    for (const auto& [x, y] : points) {
        if (is_first) {
            is_first = false;
            out << x << ","sv << y;
            continue;
        }
        out << " "sv << std::setprecision(6) << x << ","sv << y;
    }

    out << "\"";
    RenderAttrs(context.out);
    out << "/>"sv;
}

// ---------- Text --------------------

// Задаёт координаты опорной точки (атрибуты x и y)
Text& Text::SetPosition(Point pos) {
    pos_ = pos;
    return *this;
}

// Задаёт смещение относительно опорной точки (атрибуты dx, dy)
Text& Text::SetOffset(Point offset) {
    offset_ = offset;
    return *this;
}

// Задаёт размеры шрифта (атрибут font-size)
Text& Text::SetFontSize(uint32_t size) {
    font_size_ = size;
    return *this;
}

// Задаёт название шрифта (атрибут font-family)
Text& Text::SetFontFamily(std::string font_family) {
    font_family_ = font_family;
    return *this;
}

// Задаёт толщину шрифта (атрибут font-weight)
Text& Text::SetFontWeight(std::string font_weight) {
    font_weight_ = font_weight;
    return *this;
}

// Задаёт текстовое содержимое объекта (отображается внутри тега text)
Text& Text::SetData(std::string data) {
    data_ = "";
    for (const auto& symbol : data) {
        std::string str(1, symbol);
        switch (symbol) {
        case '\"': {
            str = "&quot;"sv;
            break;
        }
        case '\'': {
            str = "&apos;"sv;
            break;
        }
        case '<': {
            str = "&lt;"sv;
            break;
        }
        case '>': {
            str = "&gt;"sv;
            break;
        }
        case '&': {
            str = "&amp;"sv;
            break;
            [[fallthrough]];
        }
        default: {
            break;
        }
        }
        data_ += str;
    }

    return *this;
}

void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text";
    
    RenderAttrs(context.out);
    
    out << " x=\""sv << pos_.x << "\" y=\""sv << pos_.y << "\""sv 
        << " dx=\""sv << offset_.x << "\"" << " dy=\""sv << offset_.y << "\""sv 
        << " font-size=\""sv << font_size_ << "\""sv;
    if (!font_family_.empty()) {
        out << " font-family=\""sv << font_family_ << "\""sv;
    }

    if (!font_weight_.empty()) {
        out << " font-weight=\""sv << font_weight_ << "\""sv;
    }
    out << ">"sv;
    out << data_ << "</text>"sv;
}

// ---------- Document ----------------

// Добавляет в svg-документ объект-наследник svg::Object
void Document::AddPtr(std::unique_ptr<Object>&& obj) {
    objs.push_back(std::move(obj));
}

// Выводит в ostream svg-представление документа
void Document::Render(std::ostream& out) const {
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;
    
    RenderContext ctx(out, 2, 2);    
    for (const auto& obj : objs) {
        obj->Render(ctx);
    }
    out << "</svg>"; // << std::endl;
}

}  // namespace svg