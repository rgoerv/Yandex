#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class MoneyBox {
public:
    explicit MoneyBox(vector<int64_t> nominals)
        : nominals_(move(nominals))
        , counts_(nominals_.size()) {
    }

    const vector<int>& GetCounts() const {
        return counts_;
    }

    void PushCoin(int64_t value) {
        auto it = lower_bound(nominals_.begin(), nominals_.end(), value);
        auto position = std::distance(nominals_.begin(), it);
        ++counts_[position];
    }

    void PrintCoins(ostream& out) const {
        for(auto it = nominals_.begin(); it != nominals_.end(); ++it){
            auto count = counts_[std::distance(nominals_.begin(), it)];
            if(count)
                out << *it << ": " << count << endl;
        }
    }

private:
    const vector<int64_t> nominals_;
    vector<int> counts_;
};

ostream& operator<<(ostream& out, const MoneyBox& cash) {
    cash.PrintCoins(out);
    return out;
}

int main() {
    MoneyBox cash({10, 50, 100, 200, 500, 1000, 2000, 5000});

    int times;
    cout << "Enter number of coins you have:"s << endl;
    cin >> times;

    ifstream fin("C:\\Programs\\Yandex\\Active Code\\money.txt");

    cout << "Enter all nominals:"s << endl;
    for (int i = 0; i < times; ++i) {
        int64_t value;
        fin >> value;
        cash.PushCoin(value);
    }

    cout << cash << endl;
}