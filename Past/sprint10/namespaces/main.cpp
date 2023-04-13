#include "libstat.h"

#include <iostream>
using namespace std;
using namespace statistics::tests;

int main() {
    AggregSum();
    AggregMax();
    AggregMean();
    AggregStandardDeviation();
    AggregMode();
    AggregPrinter();

    cout << "Test passed!"sv << endl;
}