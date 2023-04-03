#include <iostream>
#include <cmath>

using namespace std;

bool IsPowOfTwo(int number)
{   
    if(number == 1) {
        return 1;
    }
    else if((number % 2) != 0 || (number <= 0)){
        return 0;
    }

    return IsPowOfTwo(number / 2);
}

int Fibonacci(int pos)
{
    if(pos == 0) {
        return 0;
    }
    else if(pos == 1) {
        return 1;
    }
    return Fibonacci(pos - 1) + Fibonacci(pos - 2);
}




int main() {
    double result = IsPowOfTwo(1023);
    cout << result << endl;
} 