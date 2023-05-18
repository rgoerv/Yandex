#include "json.h"

#include <cassert>

namespace json {

// ------------- Load Functions ------------
namespace {

Node LoadNode(std::istream& input);

Node LoadArray(std::istream& input) {
    using namespace std::literals;

    Array result;
    char c;
    for ( ; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }
    if (c != ']') {
        throw ParsingError("Load Dictonary error : end symbol dont find"s);
    }
    return Node(move(result));
}

Node LoadNumber(std::istream& input) {
    using namespace std::literals;

    std::string parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast<char>(input.get());
        if (!input) {
            throw ParsingError("Failed to read number from stream"s);
        }
    };

    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {
            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };

    if (input.peek() == '-') {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0') {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    } else {
        read_digits();
    }

    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.') {
        read_char();
        read_digits();
        is_int = false;
    }

    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E') {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-') {
            read_char();
        }
        read_digits();
        is_int = false;
    }

    try {
        if (is_int) {
            // Сначала пробуем преобразовать строку в int
            try {
                int result = std::stoi(parsed_num);
                return Node(result);
            } catch (...) {
                // В случае неудачи, например, при переполнении,
                // код ниже попробует преобразовать строку в double
            }
        }
        double result = std::stod(parsed_num);
        return Node(result);
    } catch (...) {
        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
    }
}

// Считывает содержимое строкового литерала JSON-документа
// Функцию следует использовать после считывания открывающего символа ":
Node LoadString(std::istream& input) {
    using namespace std::literals;
    
    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    std::string s;
    while (true) {
        if (it == end) {
            // Поток закончился до того, как встретили закрывающую кавычку?
            throw ParsingError("String parsing error");
        }
        const char ch = *it;
        if (ch == '"') {
            // Встретили закрывающую кавычку
            ++it;
            break;
        } else if (ch == '\\') {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end) {
                // Поток завершился сразу после символа обратной косой черты
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char) {
                case 'n':
                    s.push_back('\n');
                    break;
                case 't':
                    s.push_back('\t');
                    break;
                case 'r':
                    s.push_back('\r');
                    break;
                case '"':
                    s.push_back('"');
                    break;
                case '\\':
                    s.push_back('\\');
                    break;
                default:
                    // Встретили неизвестную escape-последовательность
                    throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        } else if (ch == '\n' || ch == '\r') {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
            throw ParsingError("Unexpected end of line"s);
        } else {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            s.push_back(ch);
        }
        ++it;
    }

    return Node(s);
}

Node LoadDict(std::istream& input) {
    using namespace std::literals;
    Dict result;
    char c;
    for (; input >> c && c != '}';) {
        if (c == ',') {
            input >> c;
        }
        std::string key = LoadString(input).AsString();
        input >> c;
        result.insert({move(key), LoadNode(input)});
    }
    if (c != '}') {
        throw ParsingError("Load Dictonary error : end symbol dont find"s);
    }

    return Node(move(result));
}

Node LoadBool(std::istream& input) {
    using namespace std::literals;
    std::string word = "";
    int i = 0;
    char c;
    for (; i < 4 && input >> c; ++i) {
        word += c;
    }
    
    if (word == "true"s) {
        return Node(true);
    }
    input >> c;
    word += c;
    if (word.substr(0, 5) == "false"s) {
        return Node(false);
    }
    throw ParsingError("Bool value not parsed"s);
}

Node LoadNull(std::istream& input) {
    using namespace std::literals;
    std::string word = "";
    int i = 0;
    for (char c; input >> c && i < 4; ++i) {
        word += c;
    }
    if (word.substr(0, 4) == "null"s) {
        return Node(nullptr);
    }
    throw ParsingError("Null are not parsed"s);
    
}

Node LoadNode(std::istream& input) {
    using namespace std::literals;
    char c;
    input >> c;

    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '"') {
        return LoadString(input);
    } else if (c == 'n') {
        input.putback(c);
        return LoadNull(input);
    } else if (c == 't' || c == 'f') {
        input.putback(c);
        return LoadBool(input);
    } else {
        input.putback(c);
        return LoadNumber(input);
    }
}

}  // namespace

Document::Document(Node root)
    : root_(std::move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Document Load(std::istream& input) {
    return Document{LoadNode(input)};
}

// --------Is() functions--------
bool Node::IsInt() const {
    return Is<int>();
}
bool Node::IsDouble() const {
    if (Is<int>()) {
        return true;
    }
    return Is<double>();
}
bool Node::IsPureDouble() const {
    return Is<double>();  
}
bool Node::IsBool() const {
    return Is<bool>();
}
bool Node::IsString() const {
    return Is<std::string>();
}
bool Node::IsNull() const {
    return Is<std::nullptr_t>();
}
bool Node::IsArray() const {
    return Is<Array>();
}
bool Node::IsMap() const {
    return Is<Dict>();
}

// --------As() functions--------
const Array& Node::AsArray() const {
    return As<Array>();
}
const Dict& Node::AsMap() const {
    return As<Dict>();
}
int Node::AsInt() const {
    return As<int>();
}
 const std::string& Node::AsString() const {
    return As<std::string>();
}
bool Node::AsBool() const {
    return As<bool>();
}
double Node::AsDouble() const {
    if (Is<int>()) {
        return static_cast<double>(As<int>());
    }
    return As<double>();
}

void PrintContext::PrintIndent() const {
    for (int i = 0; i < indent; ++i) {
        out.put(' ');
    }
}

PrintContext PrintContext::Indented() const {
    return {out, indent_step, indent_step + indent};
}

// --------------- Print Functions ----------------
// 
// Шаблон, подходящий для вывода double и int
template <typename Value>
void PrintValue(const Value& value, const PrintContext& ctx) {
    auto& out = ctx.out;
    out << value;
}

// Перегрузка функции PrintValue для вывода значений null
void PrintValue(std::nullptr_t, const PrintContext& ctx) {
    using namespace std::literals;
    auto& out = ctx.out;
    out << "null"sv;
}
// Bool value
void PrintValue(bool value, const PrintContext& ctx) {
    auto& out = ctx.out;
    out << std::boolalpha << value;
}
// String line 
void PrintValue(const std::string& line, const PrintContext& ctx) {
    auto& out = ctx.out;
    out << '\"';
    for (const auto& symbol : line) {
        char c = symbol;
        switch (c) {
            case '\n':
                out << '\\';
                c = 'n';
                break;
            case '\r':
                out << '\\';
                c = 'r';
                break;
            case '"':
                out << '\\';
                c = '\"';
                break;
            case '\\':
                out << '\\';
                c = '\\';
                break;
            default:
                break;
        }
        out << c;
    }
    out << '\"';
}
// Arrays
void PrintValue(const Array& array, const PrintContext& ctx) {
    using namespace std::literals;
    auto& out = ctx.out;
    out << '[';
    bool is_first = true;
    PrintContext next_ctx(ctx.Indented());
    for (const auto& value : array) {
        if (is_first) {
            out << '\n';
            next_ctx.PrintIndent();
            PrintNode(value, next_ctx);
            is_first = false;
            continue;
        }
        out << ",\n"sv;
        next_ctx.PrintIndent();
        PrintNode(value, next_ctx);
    }
    if (!array.empty()) {
        out << '\n';
        ctx.PrintIndent();
    }
    out << "]"sv;
}

// Maps containers
void PrintValue(const Dict& map, const PrintContext& ctx) {
    using namespace std::literals;
    auto& out = ctx.out;
    out << "{"sv;
    bool is_first = true;
    PrintContext next_ctx(ctx.Indented());
    for (const auto& [key, value] : map) {
        if (is_first) {
            out << '\n';
            next_ctx.PrintIndent();
            PrintValue(key, next_ctx);
            out << ": "sv;
            PrintNode(value, next_ctx);
            is_first = false;
            continue;
        }
        out << ",\n"sv;
        next_ctx.PrintIndent();
        PrintValue(key, next_ctx);
        out << ": "sv;
        PrintNode(value, next_ctx);
    }
    out << "\n"sv;
    ctx.PrintIndent();
    out << "}"sv;
}

// Main visitor for overloaded PrintValue
void PrintNode(const Node& node, const PrintContext& ctx) {
    std::visit(
        [&ctx](const auto& value){ PrintValue(value, ctx); },
        node.GetValue());    
} 

void Print(const Document& doc, std::ostream& output) {
    PrintNode(doc.GetRoot(), PrintContext { output, 4, 0 });
}

}  // namespace json