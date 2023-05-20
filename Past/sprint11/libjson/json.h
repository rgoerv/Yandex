#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <cassert>

namespace json {

class Node;
// Сохраните объявления Dict и Array без изменения
using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

// Эта ошибка должна выбрасываться при ошибках парсинга JSON
class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

// Контекст вывода, хранит ссылку на поток вывода и текущий отсуп
struct PrintContext {
    std::ostream& out;
    int indent_step = 4;
    int indent = 0;

    void PrintIndent() const;

    // Возвращает новый контекст вывода с увеличенным смещением
    PrintContext Indented() const;
};

class Node final 
    : public std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string> {
public:
    using Value = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

    using variant::variant;

    bool IsInt() const;
    bool IsDouble() const;
    bool IsPureDouble() const;
    bool IsBool() const;
    bool IsString() const;
    bool IsNull() const;
    bool IsArray() const;
    bool IsMap() const;


    int AsInt() const;
    bool AsBool() const;
    double AsDouble() const;
    const std::string& AsString() const;
    const Array& AsArray() const;
    const Dict& AsMap() const;
    
    const Value& GetValue() const { return *this; }

private:
    template<class Type>
    const Type& As() const {
        using namespace std::string_literals;
        if (const auto* value = std::get_if<Type>(this)) {
            assert(value != nullptr);
            return *value;
        }
        throw std::logic_error("inside contains another type value or nullptr"s);
    }

    template<class Type>
    bool Is() const {
        using namespace std::string_literals;
        if (const auto* value = std::get_if<Type>(this)) {
            assert(value != nullptr);
            return true;
        }
        return false;
    }
};

inline bool operator==(const Node& lhs, const Node& rhs) {
    return lhs.GetValue() == rhs.GetValue();
}

inline bool operator!=(const Node& lhs, const Node& rhs) {
    return lhs.GetValue() != rhs.GetValue();
}

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};

Document Load(std::istream& input);

inline bool operator==(const Document& lhs, const Document& rhs) {
    return lhs.GetRoot() == rhs.GetRoot();
}

inline bool operator!=(const Document& lhs, const Document& rhs) {
    return lhs.GetRoot() != rhs.GetRoot();
}

void Print(const Document& doc, std::ostream& output);
void PrintNode(const Node& node, const PrintContext& ctx);
}  // namespace json