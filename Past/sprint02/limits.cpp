#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>

using namespace std;

struct Document {
    long long int id;
    long long int rating;
};

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish) {
    // отсортировать документы и вывести не все    
    // const size_t max = (numeric_limits<size_t>::min() - 1);
    const size_t size = documents.size();

    if ((documents.empty()) || ((skip_start + skip_finish) > size)) { return;}
    else
    {
        sort(documents.begin(), documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.rating > rhs.rating;
            });

        for (size_t i = skip_start; i + skip_finish < size; ++i)
        {
            cout << "{ id = "s << documents[i].id << ", "s
                 << "rating = "s << documents[i].rating << " }"s << endl;
        }
    }
}

int main() {
    PrintDocuments(
    {
        {100, 5},
        {101, 7},
        {102, -4},
        {103, 9},
        {104, 1},
        {107, 2},
        {105, -2},
        {106, 3}
    },
    4,
    2
); 
}

// 103
// 101
// 100
// 106
// 107
// 104
// 105
// 102
