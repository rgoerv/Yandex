#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace literals;

class LogDuration{
public:
    LogDuration(){}

    ~LogDuration(){
        const auto end_time = steady_clock::now();
        cerr << duration_cast<milliseconds>(end_time - start_time).count() << " ms" << endl;
    }

private:
    const steady_clock::time_point start_time = steady_clock::now();
};

int main(){
    cout << "Ожидание 5 сек..." << endl;
    {
        LogDuration time_sleep;
        this_thread::sleep_for(5s);
    }
    cout << "Ожидание завершено" << endl;
}