#include "json.h"

using namespace std;

namespace json {

namespace {

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
    Array result;

    for (char c; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    return Node(move(result));
}

Node LoadInt(istream& input) {
    int result = 0;
    while (isdigit(input.peek())) {
        result *= 10;
        result += input.get() - '0';
    }
    return Node(result);
}

Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
}

Node LoadDict(istream& input) {
    Dict result;

    for (char c; input >> c && c != '}';) {
        if (c == ',') {
            input >> c;
        }

        string key = LoadString(input).AsString();
        input >> c;
        result.insert({move(key), LoadNode(input)});
    }

    return Node(move(result));
}

Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '"') {
        return LoadString(input);
    } else {
        input.putback(c);
        return LoadInt(input);
    }
}

}  // namespace


bool Node::IsInt() const {

}

bool Node::IsDouble() const {

}

bool Node::IsPureDouble() const {

}

bool Node::IsBool() const {

}

bool Node::IsString() const {

}

bool Node::IsNull() const {

}

bool Node::IsArray() const {

}

bool Node::IsMap() const {

}


Node::Node()
    : value_(nullptr) 
{
}

Node::Node(Array array)
    : value_(move(array)) 
{
}

Node::Node(Dict map)
    : value_(move(map)) 
{
}

Node::Node(bool value)
    : value_(value)
{
}

Node::Node(int value)
    : value_(value) 
{
}

Node::Node(double value)
    : value_(value)
{
}

Node::Node(string value)
    : value_(move(value)) 
{
}



const Array& Node::AsArray() const {
if (const )
    return std::visit(json::ValueGetter {}, value_);
}

const Dict& Node::AsMap() const {
    return as_map_;
}

int Node::AsInt() const {
    return as_int_;
}

const string& Node::AsString() const {
    return as_string_;
}

bool Node::AsBool() const {

}
double Node::AsDouble() const {

}



Document::Document(Node root)
    : root_(move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Document Load(istream& input) {
    return Document{LoadNode(input)};
}

void Print(const Document& doc, std::ostream& output) {
    (void) &doc;
    (void) &output;

    // Реализуйте функцию самостоятельно
}

}  // namespace json