#include <iostream>
//using namespace std;
class SearchServer {
public:
    // перенесите сюда DocumentStatus
    enum class DocumentStatus {
        ACTUAL = 0,
        IRRELEVANT = 1,
        BANNED = 2,
        REMOVED = 3,
    };
};

int main() {
    // выведите численное значение статуса BANNED
    std::cout << static_cast<int>(SearchServer::DocumentStatus::BANNED) << std::endl;
    return 0;
}

/*
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

struct Document {
    int id;
    double relevance;
    int rating;
};

void SortDocuments(vector<Document>& matched_documents) {
    sort(matched_documents.begin(), matched_documents.end(),
         [](const Document& lhs, const Document& rhs) {
            return make_pair(lhs.rating, lhs.relevance) > make_pair(rhs.rating, rhs.relevance); // automatics casual
            // return (lhs.rating > rhs.rating) || ((lhs.rating == rhs.rating) && (lhs.relevance > rhs.relevance)); // no casual very strong for boys
         });
}

int main() {
    vector<Document> documents = {{100, 0.5, 4}, {101, 1.2, 4}, {102, 0.3, 5}};
    SortDocuments(documents);
    for (const Document& document : documents) {
        cout << document.id << ' ' << document.relevance << ' ' << document.rating << endl;
    }

    return 0;
}
*/