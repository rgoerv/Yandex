#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

struct Document {
    int id;
    int relevance;
};

class SearchServer 
{
// Разместите здесь метод AddDocument и данные класса (вектор документов и множество стоп-слов)
// Приватные поля структуры/класса
    struct DocumentContent { int id = 0; vector<string> words; };
    vector<DocumentContent> documents;
    set<string> stop_words;

// Приватные методы
    
    vector<string> SplitIntoWordsNoStop(const string& text) const
    {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) 
        {
            if (stop_words.count(word) == 0) {
                words.push_back(word);
            }
        }
        return words;
    }

    set<string> ParseQuery(const string& text) const
    {
        set<string> query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) 
        {
            query_words.insert(word);
        }
        return query_words;
    }

    vector<Document> FindAllDocuments(const set<string>& query_words) const
    {
        vector<Document> matched_documents;
        for (const auto& document : documents) {
            const int relevance = MatchDocument(document, query_words);
            if (relevance > 0) {
                matched_documents.push_back({document.id, relevance});
            }
        }
        return matched_documents;
    }

public:

// Публичные методы

    string ReadLine() const 
    {
        string s;
        getline(cin, s);
        return s;
    }

    int ReadLineWithNumber() const
    {
        int result = 0;
        cin >> result;
        ReadLine();
        return result;
    }

    vector<string> SplitIntoWords(const string& text) const
    {
        vector<string> words;
        string word;
        for (const char c : text) 
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
        return words;
    }

    void SetStopWords(const string& text) 
    {
        for (const string& word : SplitIntoWords(text)) {
            stop_words.insert(word);
        }
    }


    void AddDocument(int document_id, const string& document) 
    {
        const vector<string> words = SplitIntoWordsNoStop(document);
        documents.push_back({document_id, words});
    }

    static int MatchDocument(const DocumentContent& content, const set<string>& query_words)
    {
        if (query_words.empty()) {
            return 0;
        }
        set<string> matched_words;
        for (const string& word : content.words) {
            if (matched_words.count(word) != 0) {
                continue;
            }
            if (query_words.count(word) != 0) {
                matched_words.insert(word);
            }
        }
        return static_cast<int>(matched_words.size());
    }

    vector<Document> FindTopDocuments(const string& raw_query) const
    {
        const set<string> query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(), 
        [](const Document& lhs, const Document& rhs) {
            return lhs.relevance > rhs.relevance; });

        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
};

SearchServer CreateSearchServer() 
{
    SearchServer search;
    search.SetStopWords(search.ReadLine());
    const int document_count = search.ReadLineWithNumber();

    for (int document_id = 0; document_id < document_count; ++document_id) {
        search.AddDocument(document_id, search.ReadLine());
    }

    return search;
} 


int main()
{   
    const SearchServer search = CreateSearchServer();
    // Read query
    const string query = search.ReadLine();

    // Output result
    for (auto [document_id, relevance] : search.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
    }
}
