#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <future>
#include <execution>
#include <utility>

using namespace std;

std::vector<string_view> SplitIntoWords(string_view str) {
    std::vector<string_view> result;

    size_t space = str.find_first_not_of(' ');
    size_t after_space = str.find_first_of(' ', space);

    while (space != str.npos) {
        result.push_back(str.substr(space, after_space - space));
        str.remove_prefix(std::min(str.size(), after_space));
        space = str.find_first_not_of(' ');
        after_space = str.find_first_of(' ', space);
    }
    return result;
}

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats& other) {
        for(const auto& [key, freq]: other.word_frequences){
            word_frequences[key] += freq;
        }
    }
};

using KeyWords = set<string, less<>>;

Stats ExploreLines(const KeyWords& key_words, const vector<string>&& strings) {
    Stats result;
    for_each(strings.begin(), strings.end(), [&](const string& line){ 
        for(const auto& word: SplitIntoWords(line)){
            if(key_words.count(word)) {
                result.word_frequences[(string)word] += 1;
            }
        }
    });
    return result;
}

Stats ExploreKeyWords(const KeyWords& key_words, istream& input) {
    vector<string> strings;
    strings.reserve(5001);

    Stats result;
    vector<future<Stats>> async_results;
    while(!input.eof()){
        
        while((!input.eof()) && (strings.size() < 5000)){
            string buffer;
            getline(input, buffer);
            strings.push_back(buffer);
        }
        async_results.push_back(async(ExploreLines, cref(key_words), move(strings)));
        
        strings.clear();
        strings.reserve(5001);
    }
    for_each(async_results.begin(), async_results.end(), [&result](auto& getf){ result += getf.get(); });
    return result;    
}

int main() {
    //                          6 2 1 0
    const KeyWords key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    for (const auto& [word, frequency] : ExploreKeyWords(key_words, ss).word_frequences) {
        cout << word << " " << frequency << endl;
    }

    system("pause");
    return 0;
}