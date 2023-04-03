#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <numeric>

using namespace std;

template<typename T>
ostream & operator<<(ostream & out, const vector<T> & container) {
    bool is_first = true;
    out << "[";
    for(const auto & element : container) {
        if(!is_first){
            out << ", ";
        }
        is_first = false; 
        out << element;
    }
    out << "]";
    return out;
}

template<typename T>
ostream & operator<<(ostream & out, const set<T> & container) {
    bool is_first = true;
    out << "{";
    for(const auto & element : container) {
        if(!is_first){
            out << ", ";
        }
        is_first = false;        
        out << element;
    }
    out << "}";
    return out;
}

template<typename T1, typename T2>
ostream & operator<<(ostream & out, const map<T1, T2> & container) {
    bool is_first = true;
    out << "{";
    for(const auto & [key, value] : container) {
        if(!is_first){
            out << ", ";
        }
        is_first = false;
        out << key << ": " << value;
    }
    out << "}";
    return out;
}

template <typename _T>
void RunTestImpl(_T T, const string & T_str) {
    /* Напишите недостающий код */
    T();
    cerr << T_str << " OK" << endl;
}

#define RUN_TEST(func) RunTestImpl((func), (#func)) // напишите недостающий код

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))
