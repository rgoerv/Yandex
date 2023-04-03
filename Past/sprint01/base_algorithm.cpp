// LIMITS WOW!
/*
#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

int main() {
    const auto min = numeric_limits<int64_t>::max() + 1;  // минимальное значение типа int64_t
        const auto max = numeric_limits<uint64_t>::min() - 1; // максимальное значение типа uint64_t
    int i = -1;
    unsigned j = 1;
    cout << sizeof(int) << endl;
    // cout << (i < j) << " " << static_cast<unsigned>(i) <<  endl;
    // выведите 5 чисел
    // cout << min << endl << max << endl << max + min << endl << min * 2 << endl << max * 2 << endl;
}
*/
// base struct class, methods from other theme - struct and class
/*
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

class SearchServer {
    // Содержимое раздела public: доступно для вызова из кода вне класса
public:
    int GetStopWordsSize() const {
        // Верните количество стоп-слов
        return stop_words_.size();
    }

    // Содержимое раздела private: доступно только внутри методов самого класса
private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };

    DocumentContent documents_;
    set<string> stop_words_;
};

int GetStopWordsSize(const SearchServer& server) {
    // Верните количество стоп-слов у server
    return server.GetStopWordsSize();
}

int main() {
    // 1. Создайте переменную server типа SearchServer
    SearchServer server;
    // 2. Вызовите функцию GetStopWordsSize, передав ей объект server
    cout << GetStopWordsSize(server) << endl;
    // 3. Выведите результат, возвращёный функцией GetStopWordsSize в cout
}
*/
//COUNT FROM INSIDE, NO STL
/*
#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

const int SAMPLE_COUNT = 5;

vector<int> CountNames(const set<string>& storage, const vector<string>& candidates)
{
    vector<int> output;

    for (const auto& name : candidates) 
    {
        if(storage.count(name) == 0) output.push_back(0);
        else output.push_back(1);
    }

    return output;
}

vector<int> CountNamesLong(const set<string>& storage, const vector<string>& candidates) {
    vector<int> output;
    for (auto& name : candidates) {
        output.push_back(count(storage.begin(), storage.end(), name));
    }
    return output;
}

int main() {
    set<string> s;
    vector<string> v;
    string stra = ""s;
    string strb = ""s;

    for (int j = 0; j < 10000; ++j) {
        s.insert(stra);
        stra += "a"s;
        if (j % 2 == 0) {
            v.push_back(strb);
            strb += "b"s;
        } else {
            v.push_back(stra);
        }
    }
    cout << "Testing slow version" << endl;
    for (int i = 0; i < SAMPLE_COUNT; ++i) {
        auto begin = chrono::steady_clock::now();
        CountNamesLong(s, v);
        auto end = chrono::steady_clock::now();
        cout << "Time difference Long = "s
             << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs]"s << endl;
    }
}
*/
//RELEVANTITY #FINDSYSTEM UPGRADE
/*
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#define MAX_RESULT_DOCUMENT_COUNT 5

using namespace std;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

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

set<string> ParseStopWords(const string& text) {
    set<string> stop_words;
    for (const string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words) {
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}

void AddDocument(vector<pair<int, vector<string>>>& documents, const set<string>& stop_words,
                 int document_id, const string& document) {
    const vector<string> words = SplitIntoWordsNoStop(document, stop_words);
    documents.push_back(pair<int, vector<string>>{document_id, words});
}

set<string> ParseQuery(const string& text, const set<string>& stop_words) {
    set<string> query_words;
    for (const string& word : SplitIntoWordsNoStop(text, stop_words)) {
        query_words.insert(word);
    }
    return query_words;
}

int MatchDocument(const pair<int, vector<string>>& content, const set<string>& query_words) {
    if (query_words.empty()) {
        return 0;
    }
    set<string> matched_words;
    for (const string& word : content.second) {
        if (matched_words.count(word) != 0) {
            continue;
        }
        if (query_words.count(word) != 0) {
            matched_words.insert(word);
        }
    }
    return static_cast<int>(matched_words.size());
}

// Для каждого документа возвращает его релевантность и id
vector<pair<int, int>> FindAllDocuments(const vector<pair<int, vector<string>>>& documents,
    const set<string>& query_words)
{
    // Превратите функцию FindDocuments в FindAllDocuments
    // Первым элементом возвращаемых пар идёт релевантность документа, а вторым - его id
    vector<pair<int, int>> matched_documents;
    for (const auto& document : documents) {
        const int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            matched_documents.push_back({relevance, document.first});
        }
    }
    return matched_documents;
}


// Возвращает топ-5 самых релевантных документов в виде пар: {id, релевантность}
vector<pair<int, int>> FindTopDocuments(const vector<pair<int, vector<string>>>& documents,
                                        const set<string>& stop_words, const string& raw_query) {
    // Напишите функцию, используя FindAllDocuments
    vector<pair<int, int>> Top5Document;
    vector<pair<int, int>> allDocuments_relevance = FindAllDocuments(documents, ParseQuery(raw_query, stop_words));

    sort(allDocuments_relevance.begin(), allDocuments_relevance.end());
    reverse(allDocuments_relevance.begin(), allDocuments_relevance.end());

    for(size_t i = 0; i < allDocuments_relevance.size(); ++i)
    {
        if(i >= MAX_RESULT_DOCUMENT_COUNT) break;
        Top5Document.push_back(allDocuments_relevance[i]);
    }
    
    return Top5Document;
}


int main() 
{
    const string stop_words_joined = ReadLine();
    const set<string> stop_words = ParseStopWords(stop_words_joined);

    // Read documents
    vector<pair<int, vector<string>>> documents;
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        AddDocument(documents, stop_words, document_id, ReadLine());
    }

    const string query = ReadLine();
    // Вместо FindDocuments используйте FindTopDocuments
    for (auto [relevance, document_id] : FindTopDocuments(documents, stop_words, query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
    }
}
*/
//ADDSTOPWORDS
/*
#include <set>
#include <string>
#include <numeric>

using namespace std;

string AddMinus(string accum, string stop_word)
{
    return accum + " -" + stop_word;
}

string AddStopWords(const string& query, const set<string>& stop_words) 
{
    return accumulate(stop_words.begin(), stop_words.end(), query, AddMinus);
}
*/
//ACCUMULATE
/*
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

double Average(const vector<int>& xs) {
    // посчитайте среднее арифметическое: поделите сумму на размер
    // суммирование начните с 0.0 - тогда результат выражения будет вещественным числом
    if(!(xs.empty()))
    {
        return accumulate(xs.begin(), xs.end(), 0.0)/xs.size();
    }
    return 0;
}

int main() {
    // не меняйте код main
    cout << Average({1, 2, 2}) << endl;
    return 0;
}
*/
//MEDIAN
/*
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

pair<bool, double> CalcMedian(vector<double> samples) {
    // верните {true, медиана}, если она существует,
    // то есть вектор непустой,
    // иначе - {false, 0}
    // обратите внимание - вектор принимаем по значению,
    // так как его придётся немного подпортить, чтобы вернуть ответ
    if(!samples.empty())
    {
        sort(samples.begin(), samples.end());

        size_t i;
        i = samples.size();

        if(!(i % 2 == 0))
        {
            int j = (i / 2);
            return {true, samples[j]};
        }
        else
        {
            double sum = (samples[(i / 2) - 1] + samples[(i / 2)])/2;
            return {true, sum};
        } 
    }
    return {false, 0};
}


int main() {
    int size;
    cin >> size;

    vector<double> samples;
    for (int i = 0; i < size; ++i) {
        double sample;
        cin >> sample;
        samples.push_back(sample);
    }

    pair<bool, double> result = CalcMedian(samples);
    if (result.first) {
        cout << result.second << endl;
    } else {
        cout << "Empty vector"s << endl;
    }
}
*/
//SORT REVERSE
/*
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {

    int count;
    cin >> count;
    
    vector<pair<int, string>> human_information;

    for (int i = 0; i < count; ++i) {
        string name;
        int age;
        cin >> name >> age;
        // сохраните в вектор пар
        human_information.push_back({age, name});
    }

    // выведите только имена в порядке убывания возраста
    // Jack
    // John
    // ...
    sort(human_information.begin(),human_information.end());
    reverse(human_information.begin(),human_information.end());
    for(const auto & par: human_information)
    {
        cout << par.second << endl;
    }
}
*/
//COUNT_IF
/*
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool checkAa(string str)
{
    return ((str[0] == 'a') || (str[0] == 'A'));
}
int CountStartsWithA(const vector<string>& xs) {
    // посчитайте число строк, начинающихс¤ на букву A или a
    return count_if(xs.begin(), xs.end(), checkAa);
}

int main() {
    // не меняйте тело main
    cout << CountStartsWithA({"And"s, "another"s, "one"s, "gone"s, "another"s,
                              "one"s
                              "bites"s,
                              "the"s, "dust"s});
    return 0;
}
*/
//COUNT
/*
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void PrintWorryCount(const vector<bool>& is_nervous) {
    // подсчитываем количество элементов в векторе is_nervous, равных true
    cout << count(is_nervous.begin(), is_nervous.end(), true) << endl;
}

int main() {
    // не меняйте содержимое функции main
    PrintWorryCount({true, true, false, true});
    return 0;
}
*/