#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <optional>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

optional<path> FindInclude(const path& search, const vector<path>& include_directories) {
    vector<filesystem::path> indir;

    for (const auto& path : include_directories) {
        error_code err;
        auto status = filesystem::status(path, err);
        if(status.type() == filesystem::file_type::directory) {
            for (const auto& pathindir : filesystem::directory_iterator(path)) {
                indir.push_back(pathindir.path());
            }
        } 
    }

    for(const auto& path : include_directories) {
        error_code err;
        auto status = filesystem::status(path, err);
        
        if(status.type() == filesystem::file_type::regular) {
            error_code erra_1, erra_2;
            if (filesystem::absolute(path, erra_1).lexically_normal() == filesystem::absolute(search, erra_2).lexically_normal()) {   
                return path;
            }
        }
        else if(status.type() == filesystem::file_type::directory) {
            auto path_req = FindInclude(filesystem::path(path / filesystem::path(search.filename())), indir);
            if (path_req) {
                return *path_req;
            }
        }
    }
    return nullopt;
}


bool Preprocess(const path& in_file, ostream& out, [[maybe_unused]] const vector<path>& include_directories) {
    
    ifstream in(in_file);
    if(!in) {
        return false;
    }
    
    static regex quotes(R"(\s*#\s*include\s*"([^"]*)\"\s*)");
    static regex TriaBrackets(R"(\s*#\s*include\s*<([^>]*)>\s*)");

    path currdir = in_file.parent_path();
    string line;

    for (int nline = 1; getline(in, line); ++nline) {
        smatch match;
        if (regex_match(line, match, quotes) || regex_match(line, match, TriaBrackets)) {
            path headerfile = string(match[1]);
            path inthisdir = path(currdir / headerfile);
            error_code err;
            bool status = (filesystem::status(inthisdir, err).type() == filesystem::file_type::regular);
            if (status) {
                Preprocess(inthisdir, out, include_directories);
            } else {
                auto path = FindInclude(headerfile, include_directories);
                if (path) {
                    Preprocess(*path, out, include_directories);
                } else {
                    cout << "unknown include file " << headerfile.generic_string()
                         << " at file " << in_file.generic_string() << " at line " << nline << endl;
                    return false;
                }
            }
        } else {
            out << line << endl;
        }
    }

    return true;    
}

bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories) {
    
    ifstream in(in_file);
    if(!in) {
        return false;
    }

    static regex quotes(R"(\s*#\s*include\s*"([^"]*)\"\s*)");
    static regex TriaBrackets(R"(\s*#\s*include\s*<([^>]*)>\s*)");

    ofstream out(out_file);

    path currdir = in_file.parent_path();
    string line;

    for (int nline = 1; getline(in, line); ++nline) {
        smatch match;
        if (regex_match(line, match, quotes) || regex_match(line, match, TriaBrackets)) {
            path headerfile = string(match[1]);
            path inthisdir = path(currdir / headerfile);
            error_code err;
            bool status = (filesystem::status(inthisdir, err).type() == filesystem::file_type::regular);
            if (status) {
                Preprocess(inthisdir, out, include_directories);
            } else {
                auto path = FindInclude(headerfile, include_directories);
                if (path) {
                    Preprocess(*path, out, include_directories);
                } else {
                    cout << "unknown include file " << headerfile.generic_string() 
                        << " at file " << in_file.generic_string() << " at line " << nline;
                    return false;
                }
            }
        } else {
            out << line << endl;
        }
    }
    return true;
}

string GetFileContents(string file) {
    ifstream stream(file);

    // конструируем string по двум итераторам
    return {(istreambuf_iterator<char>(stream)), istreambuf_iterator<char>()};
}

void Test() {
    error_code err;
    filesystem::remove_all("sources"_p, err);
    filesystem::create_directories("sources"_p / "include2"_p / "lib"_p, err);
    filesystem::create_directories("sources"_p / "include1"_p, err);
    filesystem::create_directories("sources"_p / "dir1"_p / "subdir"_p, err);

    {
        ofstream file("sources/a.cpp");
        file << "// this comment before include\n"
                "#include \"dir1/b.h\"\n"
                "// text between b.h and c.h\n"
                "#include \"dir1/d.h\"\n"
                "\n"
                "int SayHello() {\n"
                "    cout << \"hello, world!\" << endl;\n"
                "#   include<dummy.txt>\n"
                "}\n"sv;
    }
    {
        ofstream file("sources/dir1/b.h");
        file << "// text from b.h before include\n"
                "#include \"subdir/c.h\"\n"
                "// text from b.h after include"sv;
    }
    {
        ofstream file("sources/dir1/subdir/c.h");
        file << "// text from c.h before include\n"
                "#include <std1.h>\n"
                "// text from c.h after include\n"sv;
    }
    {
        ofstream file("sources/dir1/d.h");
        file << "// text from d.h before include\n"
                "#include \"lib/std2.h\"\n"
                "// text from d.h after include\n"sv;
    }
    {
        ofstream file("sources/include1/std1.h");
        file << "// std1\n"sv;
    }
    {
        ofstream file("sources/include2/lib/std2.h");
        file << "// std2\n"sv;
    }

    assert((!Preprocess("sources"_p / "a.cpp"_p, "sources"_p / "a.in"_p,
                                  {"sources"_p / "include1"_p,"sources"_p / "include2"_p})));

    ostringstream test_out;
    test_out << "// this comment before include\n"
                "// text from b.h before include\n"
                "// text from c.h before include\n"
                "// std1\n"
                "// text from c.h after include\n"
                "// text from b.h after include\n"
                "// text between b.h and c.h\n"
                "// text from d.h before include\n"
                "// std2\n"
                "// text from d.h after include\n"
                "\n"
                "int SayHello() {\n"
                "    cout << \"hello, world!\" << endl;\n"sv;

    assert(GetFileContents("sources/a.in"s) == test_out.str());
}

int main() {
    Test();
}
