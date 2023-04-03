#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

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
        }is_first = false;
        out << key << ": " << value;
    }
    out << "}";
    return out;
}


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


class Synonyms {
public:
    void Add(const string& first_word, const string& second_word) {
        synonyms_[first_word].insert(second_word);
        synonyms_[second_word].insert(first_word);
    }

    size_t GetSynonymCount(const string& word) const {
        if (synonyms_.count(word) != 0) {
            return synonyms_.at(word).size();
        }
        return 0;
    }

    bool AreSynonyms(const string& first_word, const string& second_word) const {
        // Напишите недостающий код
        if((!synonyms_.count(first_word)) || (!synonyms_.count(second_word))) {
            return false;
        }
        else if((synonyms_.at(first_word).count(second_word) != 0) && (synonyms_.at(second_word).count(first_word) != 0)){
            return true;
        }
        else{
            return false;
        }
    }

private:
    map<string, set<string>> synonyms_;
};

void TestAddingSynonymsIncreasesTheirCount() {
    Synonyms synonyms;
    ASSERT(synonyms.GetSynonymCount("music"s) == 0);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    ASSERT(synonyms.GetSynonymCount("music"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);

    synonyms.Add("music"s, "tune"s);
    ASSERT(synonyms.GetSynonymCount("music"s) == 2);
    ASSERT(synonyms.GetSynonymCount("tune"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);
    cerr << "TestAddingSynonymsIncreasesTheirCount OK"s << endl;
}

void TestAreSynonyms() {
    // Напишите недостающий код
    Synonyms synonyms;

    synonyms.Add("music"s, "melody"s);
    synonyms.Add("music"s, "tune"s);
    synonyms.Add("tyne"s, "track"s);

    ASSERT(synonyms.AreSynonyms("music"s, "tune"s) == 1);
    ASSERT(synonyms.AreSynonyms("music"s, "track"s) == 0);
    ASSERT(synonyms.AreSynonyms("music"s, "melody"s) == 1);
    ASSERT(synonyms.AreSynonyms("chacho"s, "morgen"s) == 0);
    cerr << "TestAreSynonyms OK"s << endl;
}

void TestSynonyms() {
    TestAddingSynonymsIncreasesTheirCount();
    TestAreSynonyms();
}

int main() {
    TestSynonyms();

    Synonyms synonyms;

    string line;
    while (getline(cin, line)) {
        istringstream command(line);
        string action;
        command >> action;

        if (action == "ADD"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            synonyms.Add(first_word, second_word);
        } else if (action == "COUNT"s) {
            string word;
            command >> word;
            cout << synonyms.GetSynonymCount(word) << endl;
        } else if (action == "CHECK"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            if (synonyms.AreSynonyms(first_word, second_word)) {
                cout << "YES"s << endl;
            } else {
                cout << "NO"s << endl;
            }
        } else if (action == "EXIT"s) {
            break;
        }
    }
}