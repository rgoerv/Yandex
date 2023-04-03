#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <set>

using namespace std;

template<typename key_, typename value_>
ostream & operator<<(ostream & out, const pair<key_, value_> & pair_)
{
    bool is_first = true;
    if(is_first){
        is_first = false; 
        out << "(" << pair_.first << ", " << pair_.second << ")";
    }
    else{
        out << " (" << pair_.first << ", " << pair_.second << ")";
    }
    return out;
}

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

template<typename Type>
ostream & operator<<(ostream & out, const set<Type> & container)
{
    out << "{"s;
    Print(out, container);
    out << "}"s;
    return out;
}

template<typename key_, typename value_>
ostream & operator<<(ostream & out, const map<key_, value_> & glossary)
{   
    out << "<"s;
    Print(out, glossary);
    out << ">"s;
    return out;
}
int main() {
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    cout << cats << endl;
    const map<string, int> cat_ages = {
    {"Мурка"s, 10}, 
    {"Белка"s, 5},
    {"Георгий"s, 2}, 
    {"Рюрик"s, 12}
    };
    cout << cat_ages << endl;
    return 0;
}