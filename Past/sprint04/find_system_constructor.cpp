#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

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

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id)
        , relevance(relevance)
        , rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:

    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
    }

    explicit SearchServer(const string& stop_words_text)
        : SearchServer(
            SplitIntoWords(stop_words_text))  // Invoke delegating constructor from string container
    {
    }

    [[nodiscard]] bool AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        vector<string> words = SplitIntoWordsNoStop(document);
        if((document_id > 0) && (GetDocumentId(document_id) == INVALID_DOCUMENT_ID)) {
            const double inv_word_count = 1.0 / words.size();
            for (const string& word : words) {
                if(!IsValidWord(word)) {
                    return false;
                }
                word_to_document_freqs_[word][document_id] += inv_word_count;
            }
            documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
            return true;
        }
        return false;
    }

    template <typename DocumentPredicate>
    [[nodiscard]] bool FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate,
                            vector<Document> & result) const {
        Query query;
        bool success = ParseQuery(raw_query, query);
        if(success) {
            auto matched_documents = FindAllDocuments(query, document_predicate);

            sort(matched_documents.begin(), matched_documents.end(),
                [](const Document& lhs, const Document& rhs) {
                    if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                        return lhs.rating > rhs.rating;
                    } else {
                        return lhs.relevance > rhs.relevance;
                    }
                });
            if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
                matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
            }
            result = matched_documents;
        }
        return success;
    }

    [[nodiscard]] bool FindTopDocuments(const string& raw_query, DocumentStatus status,
                                        vector<Document> & result) const {                                   
        return FindTopDocuments(
            raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            }, result);
    }

    [[nodiscard]] bool FindTopDocuments(const string& raw_query, vector<Document> & result) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL, result);
    }

    int GetDocumentId(int index) const {
        if((index < 0) && (index > GetDocumentCount())) {
            return INVALID_DOCUMENT_ID;
        }
        
        size_t document_index = 0;
        for(const auto & [id, document] : documents_) {
            ++document_index;
            if(index == document_index) {
                return id;
            }
        }
        return INVALID_DOCUMENT_ID;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    [[nodiscard]] bool MatchDocument(const string& raw_query, int document_id,
                        tuple<vector<string>, DocumentStatus> & result) const {
        Query query;
        bool success = ParseQuery(raw_query, query);
        if(success) {
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        result = {matched_words, documents_.at(document_id).status};
        }
        return success;
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    static bool IsValidWord(const string& word) {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    bool ParseQueryWord(string text, QueryWord & query_word) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
            if(text.empty()) {
                return false;
            }
        }
        query_word = {text, is_minus, IsStopWord(text)};
        return true;
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    bool ParseQuery(const string& text, Query & query) const {
        
        for (const string& word : SplitIntoWords(text)) {

            QueryWord query_word;
            if(ParseQueryWord(word, query_word)) {

                if (!query_word.is_stop) {
                    if (query_word.is_minus) {
                        QueryWord query_word_last;            
                        if(ParseQueryWord(query_word.data, query_word_last)){
                            if (query_word_last.is_minus) {
                                return false;
                            }
                
                        }
                        query.minus_words.insert(query_word.data);
                    } else {
                        if(!IsValidWord(query_word.data)) {
                            return false;
                        }
                        query.plus_words.insert(query_word.data);
                    }
                }
            } else {
                return false;
            }
        }
        return true;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back(
                {document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

// ==================== для примера =========================

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}
int main() {
    SearchServer search_server("and on the"s);
    // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
    // о неиспользуемом результате его вызова
    (void) search_server.AddDocument(1, "fluffy cat fluffy tail"s, DocumentStatus::ACTUAL, {7, 2, 7});
    if (!search_server.AddDocument(1, "fluffy dog and fancy collar"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
    }
    if (!search_server.AddDocument(-3, "fluffy dog and fancy collar"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
    }
    if (!search_server.AddDocument(3, "big dog star\x12ling"s, DocumentStatus::ACTUAL, {1, 3, 2})) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }
    vector<Document> documents;
    if (search_server.FindTopDocuments("--fluffy -"s, documents)) {
        for (const Document& document : documents) {
            PrintDocument(document);
        }
    } else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
} 

// int main() {
//     SearchServer search_server("и в на"s);

//     bool success;
//     vector<Document> result_find_documents;

//     search_server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, {8, -3});
//     search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
//     search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL,
//                               {5, -12, 2, 1});
//     search_server.AddDocument(3, "ухоженный скворец евгений"s, DocumentStatus::BANNED, {9});

//     cout << "ACTUAL by default:"s << endl;

//     success = search_server.FindTopDocuments("пушистый ухоженный кот"s, result_find_documents);
//     if(success) {
//         for (const Document& document : result_find_documents) {
//             PrintDocument(document);
//         }    
//     }

//     cout << "BANNED:"s << endl;

//     success = search_server.FindTopDocuments("пушистый ухоженный кот иван-чай"s, DocumentStatus::BANNED, result_find_documents);
//     if(success) {
//         for (const Document& document : result_find_documents) {
//             PrintDocument(document);
//         }
//     }

//     cout << "Even ids:"s << endl;
//     success = search_server.FindTopDocuments("пушистый ухоженный кот иван-чай"s,
//                                         [](int document_id, DocumentStatus status, int rating) {
//                                             return document_id % 2 == 0;
//                                         }, result_find_documents);
//     if(success) {
//         for (const Document &document : result_find_documents) {
//             PrintDocument(document);
//         }
//     }

//     // cout << endl << endl;

//     // for(size_t i = 0; i < search_server.GetDocumentCount(); ++i)
//     // {
//     //     const auto & [document_word, status] = search_server.MatchDocument("пушистый ухоженный кот иван-чай"s, i);
//     //     cout << i << ". ";
//     //     for(const auto word : document_word){
//     //         cout << word << " ";
//     //     }
//     //     cout << endl;
//     // }
//     return 0;
// }