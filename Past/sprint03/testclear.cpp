#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void ErrorTest(){
    ostringstream output;

    string first = "FIRTS TEST"s;
    output << first << endl;
    assert(output.str() == "FIRTS TEST\n"s);
//    output.clear(); // CLEAR

//    string last = "LAST TEST"s;
    output.str("LAST TEST\n"s);
    assert(output.str() == "LAST TEST\n"s); // Assertion failed!
    output.clear();

    cout << "ERROR TEST OK" << endl;
}

void OkTest(){
    ostringstream output_first;

    string first = "FIRTS TEST"s;
    output_first << first << endl;
    assert(output_first.str() == "FIRTS TEST\n"s);
    output_first.clear();

    ostringstream output_last;
    string last = "LAST TEST"s;
    output_last << last << endl;
    assert(output_last.str() == "LAST TEST\n"s);
    output_last.clear();

    cout << "OK TEST OK" << endl;
}

int main()
{
    OkTest();
    ErrorTest();
}