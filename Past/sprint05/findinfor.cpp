#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

void PrintSpacesPositions(string& str) {
    for(auto it = find(str.begin(), str.end(), ' '); 
        it < str.end(); it = find(next(it), str.end(), ' ')) {
        cout << distance(str.begin(), it) << endl;
    }
}

int main() {
    string str = "He said: one and one and one is three"s;
    PrintSpacesPositions(str);
    return 0;
} 

// template<typename T>
// void PrintRange(T begin, T end)
// {
//     using namespace std;
//     if(begin != end)
//     {
//         for(auto b = begin; b != end; ++b) {
//             cout << *b << " ";
//         }
//         cout << endl;
//     }
// }

// int main(){
//     using namespace std;
//     vector<int> container(10);
//     vector<int> to_erase;
//     iota(container.begin(), container.end(), 1);
//     random_shuffle(container.begin(), container.end());

//     PrintRange(container.begin(), container.end());

//     for(auto it = container.begin(); it != container.end();) {
//         auto it_next = find_if(it, container.end(), [](int element){
//             return element > 5;
//         });
//         it = it_next + 1;
//         to_erase.push_back(*it_next);
//         // cout << *it_next << endl;
//     }
//     PrintRange(to_erase.begin(), to_erase.end());
// }