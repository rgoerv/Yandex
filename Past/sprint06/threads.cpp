#include "log_duration.h"

#include <iostream>

using namespace std;

class StreamUntier {
public:
    // добавьте конструктор, деструктор
    // и дополнительные поля класса при необходимости
    
    StreamUntier(istream & stream)
    : tied_before_(stream.tie(nullptr)), thread(stream)
    {
    }

    ~StreamUntier(){
        thread.tie(tied_before_);
    }

private:
    ostream* tied_before_;
    istream & thread;
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}