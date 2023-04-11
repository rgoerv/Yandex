#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>

#include "C:\Programs\Yandex\Past\sprint09\log_duration.h"

using namespace std;

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

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

vector<string> SplitIntoWordsThread(string query) {
    //ofstream fout("C:\\Programs\\Yandex\\Active Code\\canterbury.txt"s);
    // fout << query;
    // fout.close();

    stringstream str(query);

    // ifstream fin("C:\\Programs\\Yandex\\Active Code\\canterbury.txt"s);
    
    string word;
    vector<string> words;

    while (str >> word) {
        words.push_back(word);
        word.clear();
    }
    // fin.close();
    return words;
}

int main() {

    ifstream fin("C:\\Programs\\Yandex\\Active Code\\canterbury.txt"s);

    string TEMPstrings;
    string temp;



    while(!fin.eof()) {
        fin >> temp;
        TEMPstrings += (temp + " "s);
    }

    {
        LOG_DURATION("SplitIntoWords");
        (void)SplitIntoWords(TEMPstrings);
    }

    {
        LOG_DURATION("SplitIntoWordsThread");
        (void)SplitIntoWordsThread(TEMPstrings);
    }

    {
        LOG_DURATION("SplitIntoWordsView");
        (void)SplitIntoWordsView(TEMPstrings);
    }

    system("pause");
}