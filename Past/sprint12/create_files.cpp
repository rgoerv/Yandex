#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>

using namespace std;

// реализуйте эту функцию:
void CreateFiles(const string& config_file) {
    ifstream config(config_file);
    if(config.is_open()) {
        ofstream file;
        bool is_empty = false;
        for(string line; getline(config, line); ){
            if (line[0] != '>') {
                file.close();
                file.open(line);
                is_empty = true;
            } else {
                if (is_empty) {
                    file << line.substr(1, line.npos);
                    is_empty = false;
                    continue;
                }
                file << '\n' << line.substr(1, line.npos);
            }
        }
    }
}

string GetLine(istream& in) {
    string s;
    getline(in, s);
    return s;
}

int main() {
    ofstream("test_config.txt"s) << "a.txt\n"
                                    ">10\n"
                                    ">abc\n"
                                    "b.txt\n"
                                    ">123"sv;

    CreateFiles("test_config.txt"s);
    ifstream in_a("a.txt"s);
    assert(GetLine(in_a) == "10"s && GetLine(in_a) == "abc"s);

    ifstream in_b("b.txt"s);
    assert(GetLine(in_b) == "123"s);
}
