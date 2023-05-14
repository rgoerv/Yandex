#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>

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

struct ValueGetter {
    std::nullptr_t operator()(std::nullptr_t) const {
        return nullptr;
    }
    const Array& operator()(const Array& array) const {
        return array;
    }
    const Dict& operator()(const Dict& map) const {
        return map;
    }
    bool operator()(bool value) const {
        return value;
    }
    int operator()(int value) const {
        return value;
    }
    double operator()(double value) const {
        return value;
    }
    const std::string& operator()(const std::string& value) const {
        return value;
    }
};


struct ValueChecker {
    bool operator()(std::nullptr_t) const {
        return true;
    }
    bool operator()(const Array& array) const {
        return true;
    }
    bool operator()(const Dict& map) const {
        return true;
    }
    bool operator()(bool value) const {
        return true;
    }
    bool operator()(int value) const {
        return true;
    }
    bool operator()(double value) const {
        return true;
    }
    bool operator()(const std::string& value) const {
        return true;
    }
};



class Node {
public:
    using Value = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

    explicit Node();
    explicit Node(Array array);
    explicit Node(Dict map);
    explicit Node(bool value);
    explicit Node(int value);
    explicit Node(double value);
    explicit Node(std::string value);

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
    
    const Value& GetValue() const { return value_; }

private:
    Value value_;
};

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};

Document Load(std::istream& input);

void Print(const Document& doc, std::ostream& output);

}  // namespace json