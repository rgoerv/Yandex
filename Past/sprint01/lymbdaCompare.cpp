#include <iostream>
#include <vector>

using namespace std;

// string ReadLine() {
//     string s;
//     getline(cin, s);
//     return s;
// }

int ReadLineWithNumber() {
    int result;
    cin >> result;
    //ReadLine();
    return result;
}

int main()
{
    vector<int> rating;
    rating.push_back(ReadLineWithNumber());
    for (size_t i = 0; i < rating[0]; ++i) {
        rating.push_back(ReadLineWithNumber());
    }


    for(const int & rate: rating) cout << rate << " ";

    cout << endl;
}


// POG MOMENT LYMBDA FUNCTION WTF??? 7 > 20+ strs
/*
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs, const map<string, int>& max_amount,
                       map<string, int>& shelter) {
    return count_if(new_dogs.begin(), new_dogs.end(), 
    [& max_amount, & shelter](const string & new_dog){
        size_t i = max_amount.at(new_dog);
        if((shelter[new_dog] < i))
        {
            ++shelter[new_dog];
            return true;
        }                       
        return false;
    });
}

int main() {
    map<string, int> shelter = {{"landseer"s, 1}, {"otterhound"s, 2}, {"pekingese"s, 2}, {"pointer"s, 3}};
    const map<string, int> max_amount = {{"landseer"s, 2}, {"otterhound"s, 3}, {"pekingese"s, 4}, {"pointer"s, 7}};
    const vector<string>& new_dogs = {"landseer"s, "otterhound"s, "otterhound"s, "otterhound"s, "pointer"s};
    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter) << endl;
    return 0;
}
*/
// VALUE CAPTURE
/*
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

int main() {
    const int queryCount = ReadLineWithNumber();

    vector<string> queries(queryCount);
    for (string& query : queries) {
        query = ReadLine();
    }
    const string buzzword = ReadLine();

    cout << count_if(queries.begin(), queries.end(), 
    [buzzword](const string& query) {
        // Реализуйте эту лямбда-функцию
        // Верните true, если query содержит слово buzzword
    {
        vector<string> words;
        string word;
        for (const char c : query) 
        {
            if (c == ' ') {
                if (!word.empty()) {
                    words.push_back(word);
                    word.clear();
                }
            } else { word += c; }
        }
        if (!word.empty()) 
        {
            words.push_back(word);
        }
        
        if(count(words.begin(), words.end(), buzzword) > 0) return true;

        return false;
    }
    });
    cout << endl;
}
*/
// lymbda compare from sort to up to middle from begin to low to middle from end
/*
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

vector<int> ReadNumbers() {
    int count;
    cin >> count;

    vector<int> numbers;
    for (int i = 0; i < count; ++i) {
        int number;
        cin >> number;
        numbers.push_back(number);
    }

    return numbers;
}

void PrintNumbers(const vector<int>& numbers) {
    for (auto number : numbers) {
        cout << number << " "s;
    }
}

int main() {
    vector<int> numbers = ReadNumbers();

    if(!numbers.empty())
    {
    sort(numbers.begin(), numbers.end(), [](int n1, int n2) {
        // 
        //    Напишите тело лямбда-функции самостоятельно.
        //    Сначала должны располагаться чётные числа в порядке возрастания,
        //    а следом за ними — нечётные числа в порядке убывания
        // 
        if((n1 % 2 == 0) && ( n2 % 2 == 0)){
            return n1 < n2;    
        }
        else if((n1 % 2 != 0) && ( n2 % 2 != 0)){
            return n1 > n2;
        }
        else if((n1 % 2 != 0) && ( n2 % 2 == 0)){
            return false;
        }
        else if((n1 % 2 == 0) && ( n2 % 2 != 0)){
            return true;
        }
        else return true;     
    });
    }
    PrintNumbers(numbers);
}
*/
// lexicographical_compare lymbda in 2 degree 
/*
#include <algorithm>0, buzzword.length(), 
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

int main() 
{
    // считайте входные данные и сформируйте вывод программы
    // 8 quantity AMOUNT Selection Search 4ever music Mickey market

    size_t count_words = 0;
    cin >> count_words;

    string word;
    vector<string> words;
    for(size_t i = 0; i < count_words; ++i) {
        cin >> word;
        words.push_back(word);
    }  

    sort(words.begin(), words.end(), 
    [](const string & lhs, const string & rhs){
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
    []( const char & a , const char & b ) { return tolower(a) < tolower(b); }); });

    for(const auto & word: words)
    {
        cout << word << " ";
    }
    cout << endl;
}
*/