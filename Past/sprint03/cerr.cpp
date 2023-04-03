#include <iostream>
#include <string>

using namespace std;

template <typename _T>
void RunTestImpl(_T T, const string & T_str) {
    /* Напишите недостающий код */
    T();
    cerr << T_str << " OK" << endl;
}

#define RUN_TEST(func) RunTestImpl((func), (#func)) // напишите недостающий код

void Test1() {
//    abort();
}

int main() {
    RUN_TEST(Test1);
}