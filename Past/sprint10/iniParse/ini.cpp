#include <string>
#include <string_view>
#include <utility>
#include <sstream>
#include <fstream>
#include <iostream>

#include "ini.h"

using namespace std;

// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini

namespace ini {

using namespace std;

Section& Document::AddSection(std::string name) {
    return sections_[name];
}

const Section& Document::GetSection(const std::string& name) const {
    static Section tmp;
    return sections_.count(name) > 0 ? sections_.at(name) : tmp;
}

std::size_t Document::GetSectionCount() const {
    return sections_.size();
}

namespace detail {

const string ParseHeader(string_view line) {
    int64_t pos_quad_staple = line.find('[');
    int64_t begin = line.find_first_not_of('[', pos_quad_staple);
    return (string)line.substr(begin, line.find(']') - begin);
}

const pair<string, string> ParseVariable(string_view line) {
    
    // Cюда не попадут пустые строки и строки без знака равно

    int64_t npos = line.npos;

    // Находим первую букву названия переменной
    int64_t begin_name_pos = line.find_first_not_of(' ');
    line.remove_prefix(begin_name_pos);

    // Находим конец название переменной
    // Она равна пробелу или символу '='
    int64_t pos_equal = line.find('='); // Напоминаю, он точно есть
    int64_t next_space = line.find(' ');
    // cout << line << endl;
    // cout << "pos = : " << pos_equal << " pos space : " << next_space << endl;

    string_view variable_name;

    // Если знак равенства находится после пробела
    variable_name = (next_space == npos) || (next_space > pos_equal) 
                    ? line.substr(0, pos_equal) : line.substr(0, next_space);
    line.remove_prefix(pos_equal + 1);

    // cout << line << endl;

    int64_t pos_value_begin = line.find_first_not_of(' ');
    int64_t pos_value_end = line.find_last_not_of(' ') + 1;

    // cout << "value begin: " << pos_value_begin << " = " << line[pos_value_begin] 
    //    << " value end: " << pos_value_end << " = " << line[pos_value_end] << endl;

    string_view variable_value = pos_value_begin == npos ? "" 
                : line.substr(pos_value_begin, pos_value_end - pos_value_begin);

    // cout << variable_value << "/n" << endl << endl;

    return { static_cast<string>(variable_name), static_cast<string>(variable_value) };
}
} // namespace detail


Document Load(std::istream& input) {
    Document result;
    string section_name;
    for (string line; getline(input, line);) {
        if (!line.size()) {
            continue;
        }
        if (line.find('[') != line.npos) {
            section_name = detail::ParseHeader(line);
            result.AddSection(section_name);
            // cout << "/n" << section_name << "/n" << endl;
            continue;
        }
        int64_t is_valid = line.find('=');
        if(is_valid == line.npos) { continue; }

        const auto& var = detail::ParseVariable(line);
        result.GetSections()[section_name][var.first] = var.second;
        // cout << "/n" << section_name << "/n" << var.first << "/n" << " = " << "/n" << var.second << "/n" << endl;
    }
    return result;
}
} // namespace ini