#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

template<typename T>
void PrintRange(T begin, T end)
{
    if(begin != end)
    {
        for(auto b = begin; b != end; ++b)
        {
            cout << *b << " ";
        }
        cout << endl;
    }
}

template<typename T, typename K>
void FindAndPrint(const T & container, const K & type_container)
{
    auto it = find_if(container.begin(), container.end(), [& type_container](const K & element){
        return element == type_container;
    });
    PrintRange(container.begin(), it);
    PrintRange(it, container.end());
}

template <typename It>
auto MakeVector(It range_begin, It range_end) {
    return vector(range_begin, range_end);
}

template<typename Container, typename Pos>
void EraseAndPrint(Container & container, Pos iterator_one, Pos iterator_first, Pos iterator_last)
{
    container.erase(container.begin() + iterator_one);
    PrintRange(container.begin(), container.end());

    container.erase(container.begin() + iterator_first, container.begin() + iterator_last);
    PrintRange(container.begin(), container.end());
}

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    EraseAndPrint(langs, 0, 0, 2);
    return 0;
} 