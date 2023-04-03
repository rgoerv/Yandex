#include <iostream>
#include <string>

using namespace std;

class House {
public:
    House() {
        cout << "+house"s;
    }
    ~House() {
        cout << "-house"s;
    }



};

class Tree {
public:
    Tree() {
        cout << "+tree"s;
    }
    ~Tree() {
        cout << "-tree"s;
    }

    House house1;
    House house2;
};



House BuildHouse(Tree tree) {
    return House{};
}


int main() {
    cout << "main";
    House house = BuildHouse({});
    cout << "~life"s;
}