#include <iostream>
#include <string>

#include "log_duration.h"

using namespace std;

// template <typename F>
// string BruteForceInternal(F check, const string& begin, int n){
//     if(n == 5) {
//         return check(begin) ? begin : ""s;
//     }
//     if(n < 5) {

//         BruteForce(check, begin, ++n);
//     }

// }

template <typename F>
string BruteForce(F check) {
    // верните строку str, для которой check(str) будет true
    LOG_DURATION("BruteForce");
    string brute = "AAAAA"s;

    for(char i = 'A'; i <= 'Z'; ++i){
        for(char j = 'A'; j <= 'Z'; ++j){
            for(char k = 'A'; k <= 'Z'; ++k){
                for(char l = 'A'; l <= 'Z'; ++l){
                    for(char m = 'A'; m <= 'Z'; ++m){
                        brute[4] = m;
                        if(check(brute)) return brute;
                        //cout << brute << endl;
                    }
                    brute[3] = l;
                    if(check(brute)) return brute;
                    //cout << brute << endl;
                }
                brute[2] = k;
                if(check(brute)) return brute;
                //cout << brute << endl;
            }
            brute[1] = j;
            if(check(brute)) return brute;
            //cout << brute << endl;  
        }
        brute[0] = i;
        if(check(brute)) return brute;
        //cout << brute << endl;
    }               
    
    return brute;
}

int main() {
    string pass = "ZZZZZ"s;
    auto check = [pass](const string& s) {
        return s == pass;
    };
    cout << BruteForce(check) << endl;
}