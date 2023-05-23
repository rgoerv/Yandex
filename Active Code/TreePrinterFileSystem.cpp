#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

void PrintTree(ostream& dst, const path& p, const string& indent) {
    dst << indent << p.filename().string() << endl;
    
    std::vector<filesystem::directory_entry> objects;
    
    for(const auto& path : filesystem::directory_iterator(p)) {
        objects.push_back(path);
    }
    sort(objects.begin(), objects.end(), [](const filesystem::directory_entry& lhs, const filesystem::directory_entry& rhs){
        return lhs.path().string() > rhs.path().string();
    });

    vector<string> files;

    for(const auto& path : objects) {
        if(path.is_directory()) {
            PrintTree(dst, path, indent + "  "s);
            continue;
        }
        files.push_back(path.path().filename().string());
    }
    
    for(const auto& path : files) {
        dst << "  "s + indent << path << endl;
    }
}


void PrintTree(ostream& dst, const path& p) {
    PrintTree(dst, p, ""s);
}

int main() {
    error_code err;
    filesystem::remove_all("test_dir", err);
    filesystem::create_directories("test_dir"_p / "a"_p, err);
    filesystem::create_directories("test_dir"_p / "b"_p / "c"_p, err);
    filesystem::create_directories("test_dir"_p / "b"_p, err);

    ofstream("test_dir"_p / "b"_p / "f1.txt"_p);
    ofstream("test_dir"_p / "b"_p / "f2.txt"_p);
    ofstream("test_dir"_p / "a"_p / "f3.txt"_p);
    ofstream("test_dir"_p / "b"_p / "c"_p / "f4.txt"_p);

    ostringstream out;
    PrintTree(cout, "test_dir"_p);
    assert(out.str() ==
        "test_dir\n"
        "  b\n"
        "    f2.txt\n"
        "    f1.txt\n"
        "  a\n"
        "    f3.txt\n"sv
    );
}
