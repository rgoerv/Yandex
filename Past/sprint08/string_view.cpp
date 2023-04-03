#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>
#include <algorithm>

using namespace std;

vector<string_view> SplitIntoWordsView(string_view str) {
    vector<string_view> result;
    int64_t space = str.find_first_not_of(' ');
    int64_t pos_end = str.npos;
    while (space != pos_end) {
        space = str.find(' ');
        if(space != 0) {
            result.push_back(space == pos_end ? str.substr(0, distance(str.begin(), str.end())) : str.substr(0, space));
        }
        str.remove_prefix(std::min(str.size(), (size_t)space));
        space = str.find_first_not_of(' ');
        if(space != 0) {
            str.remove_prefix(std::min(str.size(), (size_t)space));
        }
    }
    return result;
}

int main() {
    assert((SplitIntoWordsView("") == vector<string_view>{}));
    assert((SplitIntoWordsView("     ") == vector<string_view>{}));
    assert((SplitIntoWordsView("aaaaaaa") == vector{"aaaaaaa"sv}));
    assert((SplitIntoWordsView("a") == vector{"a"sv}));
    assert((SplitIntoWordsView("a b c") == vector{"a"sv, "b"sv, "c"sv}));
    assert((SplitIntoWordsView("a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    cout << "All OK" << endl;
}