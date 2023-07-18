#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <utility>
#include <iomanip>
#include <numeric>

using namespace std;

class ReadersProcessor {
public:
    ReadersProcessor() {
        user_page_.resize(100'000u);
        count_page_user_.resize(1000u);
    }

    void Read() {
        int64_t name = 0;
        cin >> name;
    
        int64_t page = 0;
        cin >> page;

        if(user_page_[name] == 0) {
            ++size;
        }
        else {
            --count_page_user_[user_page_[name]];
        }
        user_page_[name] = page;
        ++count_page_user_[page];      
    }

    double ComputeShare(int name) const {
        return static_cast<double>(std::reduce(count_page_user_.begin(), count_page_user_.begin() + user_page_[name])) 
                / static_cast<double>(size - 1);
    }

    void Cheer() {
        int64_t name = 0;
        cin >> name;

        if(size == 1 && user_page_[name] != 0) {
            cout << 1 << endl;
        }
        else if(user_page_[name] == 0) {
            cout << 0 << endl;
        }
        else {
            cout << setprecision(6) << ComputeShare(name) << endl;
        }
    }

private:
    vector<int> user_page_;
    vector<int> count_page_user_;
    int64_t size = 0;
};


int main() {
    int64_t queries;
    cin >> queries;

    ReadersProcessor readers;

    for( ; queries > 0; --queries) {
        string query;
        cin >> query;
        if(query == "READ"s) {
            readers.Read();
        }
        else if(query == "CHEER"s) {
            readers.Cheer();
        }
    }
    cout << endl;
}