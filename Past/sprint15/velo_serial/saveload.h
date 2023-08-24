#include <iostream>
#include <map>
#include <string>
#include <vector>

// Serialization

template <typename T>
void Serialize(T val, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(&val), sizeof(val));
}

inline void Serialize(const std::string& str, std::ostream& out) {
    Serialize(str.size(), out);
    out.write(reinterpret_cast<const char*>(str.data()), str.size());
}

template <typename T>
void Serialize(const std::vector<T>& val, std::ostream& out) {
    Serialize(val.size(), out);
    for(const auto& value : val) {
        Serialize(value, out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& val, std::ostream& out) {
    Serialize(val.size(), out);
    for(const auto& [key, value] : val) {
        Serialize(key, out);
        Serialize(value, out);
    }
}

// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& val) {
    in.read(reinterpret_cast<char*>(&val), sizeof(val));
}

inline void Deserialize(std::istream& in, std::string& str) {
    size_t size = 0;
    Deserialize<std::size_t>(in, size);
    str.resize(size);
    in.read(reinterpret_cast<char*>(str.data()), str.size());
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& val) {
    size_t size = 0;
    Deserialize<size_t>(in, size);
    val.resize(size);
    for(auto& value : val) {
        Deserialize(in, value);
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& val) {
    size_t size = 0;
    Deserialize<size_t>(in, size);

    for(auto it = 0; it < size; ++it) {
        T1 key;
        T2 value;
        Deserialize(in, key);
        Deserialize(in, value);
        val.insert({key, value});
    }
}

// Поскольку функции шаблонные, их реализации будем писать прямо в h-файле.
// Не забудьте объявить нешаблонные функции как inline.