#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

vector<string_view> SplitIntoStops(string_view line, const char delim)
{
    cout << "begin" << endl;
    vector<string_view> result;

    size_t stopname_end = line.find(delim);

    while (stopname_end != line.npos) {
        result.push_back(line.substr(0, stopname_end - 1));
        cout << line.substr(0, stopname_end - 1) << endl;
        line.remove_prefix(stopname_end + 2);
        stopname_end = line.find(delim);
    }

    result.push_back(line.substr(0, line.find_last_not_of(' ') + 1));
    return result;
}

void SplitIntoStopsTest(){
    string_view str1 = "Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye"sv;
    assert((SplitIntoStops(str1, '>') == vector<string_view>{
        "Biryulyovo Zapadnoye"sv, "Biryusinka"sv, "Universam"sv, "Biryulyovo Tovarnaya"sv, 
        "Biryulyovo Passazhirskaya"sv,
        "Biryulyovo Zapadnoye"sv
    }));

    string_view str2 = "Biryulyovo Zapadnoye > Biryusinka "sv;
    assert((SplitIntoStops(str2, '>') == vector<string_view>{
        "Biryulyovo Zapadnoye"sv, "Biryusinka"sv
    }));

    string_view str3 = "Biryulyovo Zapadnoye - Biryusinka - Universam "sv;
    assert((SplitIntoStops(str3, '-') == vector<string_view>{
        "Biryulyovo Zapadnoye"sv, "Biryusinka"sv, "Universam"sv
    }));
}



int main() {
    // string word("oneboba twoboba threeboba"s);
    // string_view sv_word = word;
    // cout << sv_word << endl;
    // cout << sv_word.substr(0, sv_word.find('e')) << endl;  
    // cout << sv_word.substr(0, sv_word.size() - 1) << endl;
    // sv_word.remove_prefix(8);
    // cout << sv_word << endl;
    // sv_word.remove_suffix(1);
    // cout << sv_word << endl;
    SplitIntoStopsTest();
    cout << "Success" << endl;
}