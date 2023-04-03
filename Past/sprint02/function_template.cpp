#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>

using namespace std;

template<typename TermInvers, typename Documents>
vector<double> ComputeTfIdfs(const Documents & documents, 
                                                            const TermInvers & term)
{
    vector<double> results_;

    const double idf = log(documents.size() * 1.0 / count_if(documents.begin(), documents.end(), 
    [&term](const auto & document){
        return count(document.begin(), document.end(), term) > 0;
    }));


    for(const auto & document : documents)
    {   
        const double tf = count(document.begin(), document.end(), term) / (double)document.size();
        results_.push_back(tf*idf);
    }
    
    return results_;
}

int main() {

    const vector<vector<string>> documents = {
        {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
        {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
        {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
    };

    const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);

    for (const double tf_idf : tf_idfs) {
        cout << tf_idf << " "s;
    }

    cout << endl;
    return 0;
}
//
/*
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

template <typename Term>
map<Term, int> ComputeTermFreqs(const vector<Term>& terms) {
    map<Term, int> term_freqs;
    for (const Term& term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}

pair<string, int> FindMaxFreqAnimal(const vector<pair<string, int>>& animals) {
    // верните животного с максимальной частотой
    pair<string, int> max_element = {"", 0};
    for(const auto & val : ComputeTermFreqs(animals))
    {
        if(val.second > max_element.second)
        {
            max_element = val.first;
        }
    }
    return max_element;
}

int main() {
    const vector<pair<string, int>> animals = {
        {"Murka"s, 5},  // 5-летняя Мурка
        {"Belka"s, 6},  // 6-летняя Белка
        {"Murka"s, 7},  // 7-летняя Мурка не та же, что 5-летняя!
        {"Murka"s, 5},  // Снова 5-летняя Мурка
        {"Belka"s, 6},  // Снова 6-летняя Белка
    };
    const pair<string, int> max_freq_animal = FindMaxFreqAnimal(animals);
    cout << max_freq_animal.first << " "s << max_freq_animal.second << endl;
}
*/