#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

enum class Status { ACTUAL, EXPIRED, DELETED };

struct Document {
    int id;
    Status status;
    double relevance;
    int rating;

    auto MakeKey() const
    {
        return make_tuple(status, -rating, -relevance);
    }
};

void SortDocuments(vector<Document>& matched_documents) {
    sort(matched_documents.begin(), matched_documents.end(),
        [](const Document& lhs, const Document& rhs) {
            return lhs.MakeKey( ) < rhs.MakeKey();
            // return make_tuple(rhs.status, lhs.rating, lhs.relevance) > make_tuple(lhs.status, rhs.rating, rhs.relevance);
            // return (lhs.status < rhs.status) || ((lhs.status == rhs.status) && (make_pair(hs.rating, lhs.relevance) > make_pair(rhs.rating, rhs.relevance)));
            // return (lhs.status > rhs.status) < (make_pair(lhs.rating, lhs.relevance) > make_pair(rhs.rating, rhs.relevance));
        });
}

int main() {
    vector<Document> documents = {
        {100, Status::ACTUAL, 0.5, 4}, {101, Status::EXPIRED, 0.5, 4},
        {102, Status::ACTUAL, 1.2, 4}, {103, Status::DELETED, 1.2, 4},
        {104, Status::ACTUAL, 0.3, 5},
    };
    SortDocuments(documents);
    for (const Document& document : documents) {
        cout << document.id << ' ' << static_cast<int>(document.status) << ' ' << document.relevance
             << ' ' << document.rating << endl;
    }

    return 0;
}