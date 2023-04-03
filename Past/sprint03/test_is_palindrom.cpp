#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

// Определяет, будет ли слово палиндромом
// text может быть строкой, содержащей строчные буквы английского алфавита и пробелы
// Пустые строки и строки, состоящие только из пробелов, - это не палиндромы

template<typename TypeDocument>
void Print(ostream & out, const TypeDocument & container)
{
    bool is_first = true;
    for(const auto & value : container)
    {
        if(!is_first) out << ", ";
        out << value;
        is_first = false;
    }
}


template<typename Type>
ostream & operator<<(ostream & out, const vector<Type> & container)
{
    out << "["s;
    Print(out, container);
    out << "]"s;
    return out;
}

bool IsPalindrome(const string& text) {
    // Напишите недостающий код
    vector<char> split;
    for(const auto c : text) {
        if(c == ' ') {
            continue;
        }
        else {
            split.push_back(c);
            // cout << c;
        }
    }
    // cout << endl;

    if(split.empty()){
        return false;
    }
    else{
        const string not_reverse = accumulate(split.begin(), split.end(), ""s);

        // cout << "split not reverse : " << not_reverse << endl;

        reverse(split.begin(), split.end());

        // cout << "split revere : " << split << endl;

        // cout << "accumelate : " << accumulate(split.begin(), split.end(), ""s) << endl;

        if(not_reverse == accumulate(split.begin(), split.end(), ""s)) {
            return true;
        }
        else { 
           return false;
        }
    }
}

void TestIsPalindrome()
{
    assert(IsPalindrome("radar") == true);
    assert(IsPalindrome("never odd or even") == true);
    assert(IsPalindrome("apple") == false);
    assert(IsPalindrome("kick") == false);
    assert(IsPalindrome("       ") == false);
    assert(IsPalindrome("") == false);
    assert(IsPalindrome("aaaaaza") == false);
    assert(IsPalindrome("хахах") == true);

    cout << "TestIsPalindrome OK" << endl;
}



int main() {

    TestIsPalindrome();

    string text;
    getline(cin, text);

    if (IsPalindrome(text)) {
        cout << "palindrome"s << endl;
    } else {
        cout << "not a palindrome"s << endl;
    }
}