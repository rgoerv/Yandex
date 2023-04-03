#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <fstream>

using namespace std;

vector<pair<string, int>> GetSortedWordCounts(vector<string> words) {
    map<string, int> counts_map;

    for (auto& word : words) {
        ++counts_map[move(word)];
    }

    vector<pair<string, int>> counts_vector(move_iterator(counts_map.begin()), move_iterator(counts_map.end()));
    sort(counts_vector.begin(), counts_vector.end(), [](const auto& l, const auto& r) {
        return l.second > r.second;
    });

    return counts_vector;
}

int main() {
    vector<string> words;
    string word;

    ifstream filestream("canterbury.txt");

    if (!filestream.is_open())
        std::cout << "failed to open " << "canterbury.txt" << '\n';
    else 
        std::cout << "open " << "canterbury.txt" << '\n';

    while (!filestream.eof()) {
        filestream >> word;
        cout << word << flush;
        word.size() == 5 ? (void)words.push_back(word) : (void)0;
    }

    auto counts_vector = GetSortedWordCounts(move(words));

    cout << "Слово - Количество упоминаний в тексте"s << endl;
    // выводим первые 10 слов
    for (auto [iter, i] = tuple(counts_vector.begin(), 0); i < 10 && iter != counts_vector.end(); ++i, ++iter) {
        cout << iter->first << " - "s << iter->second << endl;
    }
}