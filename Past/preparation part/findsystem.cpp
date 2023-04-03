#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>

using namespace std;

set<string> MakeSet(vector<string> query_words) 
{
    set<string> s(query_words.begin(), query_words.end());
    return s;
}

string ReadLine() 
{
    string str;
    getline(cin, str);
    return str;
}

int ReadLineWithNumber() 
{
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) 
{
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

set<string> ParseStopWords(const string& text) 
{
    set<string> stop_words;
    for (const string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words) 
{
    vector<string> words;
    // проходим по всем словам из текста и проверяем, что их нет в списке стоп-слов
    for (const string& word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    // вернём результат без стоп-слов
    return words;
}

void AddDocument(vector<pair<int, vector<string>>>& documents,
                 const set<string>& stop_words,
                 int document_id, 
                 const string& document) 
{
    const vector<string> words = SplitIntoWordsNoStop(document, stop_words);
    documents.push_back({document_id, words});
}

// Разбирает text на слова и возвращает только те из них, которые не входят в stop_words
set<string> ParseQuery(const string& text, const set<string>& stop_words) 
{
    set<string> query_words = MakeSet(SplitIntoWordsNoStop(text, stop_words));
    // Напишите код функции
    return query_words;
}

// Возвращает количество слов документа (document_words)
// встречающихся в словах поискового запроса query_words
int MatchDocument(const pair<int, vector<string>>& content, 
                  const set<string>& query_words)  
{
    // Напишите код функции
    int count = 0;

    for(const auto & document_text : content.second)
    {
        if(query_words.count(document_text)) ++count;
    }

    return count;
}

// Возвращает массив id документов, подходящих под запрос query
// Стоп-слова исключаются из поиска
vector<pair<int, int>> FindDocuments(const vector<pair<int, vector<string>>>& documents,
                                     const set<string>& stop_words,
                                     const string& query) 
{
    vector<pair<int, int>> matched_documents;
    size_t i = 0;
    // Напишите код функции
    // Воспользуйте вспомогательными функциями ParseQuery, MatchDocument
    // В качестве id документа используйте его индекс в массиве documents
    for(const auto & document_text : documents){
        if(MatchDocument(document_text, ParseQuery(query, stop_words))) 
        matched_documents.push_back({i, MatchDocument(document_text, ParseQuery(query, stop_words))});
        i++;
    }

    return matched_documents;
}

int main() 
{
    const string stop_words_joined = ReadLine();
    const set<string> stop_words = ParseStopWords(stop_words_joined);

    // Считываем документы
    vector<pair<int, vector<string>>> documents;
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        AddDocument(documents, stop_words, document_id, ReadLine());
    }

    const string query = ReadLine();
    // Выводим результаты поиска по запросу query
    for (auto [document_id, relevance] : FindDocuments(documents, stop_words, query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
    }
} 