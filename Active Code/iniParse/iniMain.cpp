#include <cassert>
#include <sstream>
#include <fstream>

// в этой задаче ситуация обратная:
// меняйте только файлы ini.cpp, ini.h
// а main.cpp трогать не нужно
#include "ini.h"

using namespace std;

int main() {
    std::istringstream input{
       "[first]\n"
        "a=  b\n"
        "b  =a\n"
        "c  =           c\n"
        "   [second]\n"
        " a=b\n"
        "    b=a\n"
        "[vegetables]\n"
        "potatoes=10\n"
        "onions=1 \n"
        "razzberry   =  1 \n"
        "blueberry   =12312 \n"
        "cherry=     \n"
        "   orange       =       67    \n"
        "=\n"
        "\n"
        "cucumbers=12\n"
        "\n"
        "[guests] \n"
        "guest1_name = Ivan Durak\n"
        "guest2_name =  Vasilisa Premudraya\n"
        "    guest3_name=Vasilisa Premudraya    Premudraya  Premudraya   \n"
        "[guest black list]"};

    // std::ifstream ininput("C:\\Programs\\Yandex\\Active Code\\iniParse\\input.ini"s);
    // if(ininput.is_open()) cout << "ininput open" << endl;

    ini::Document doc = ini::Load(input);

    assert((doc.GetSection("first") == ini::Section{
        {"a"s, "b"s},
        {"b"s, "a"s},
        {"c"s, "c"s}
    } ));
    assert((doc.GetSection("second") == ini::Section{
        {"a"s, "b"s},
        {"b"s, "a"s}
    } ));

    assert(doc.GetSectionCount() == 5);
    assert((doc.GetSection("vegetables"s)
            == ini::Section{
                {"potatoes"s, "10"s},
                {"onions"s, "1"s},
                {"cucumbers"s, "12"s},
                {"razzberry"s, "1"s},
                {"blueberry"s, "12312"s},
                {"cherry"s, ""s},
                {"orange", "67"s},
                {"", ""s}
            }));
    assert((doc.GetSection("guests"s)
            == ini::Section{
                {"guest1_name"s, "Ivan Durak"s}, 
                {"guest2_name"s, "Vasilisa Premudraya"s},
                {"guest3_name"s, "Vasilisa Premudraya    Premudraya  Premudraya"s}
            }));
    assert((doc.GetSection("dragons"s) == ini::Section{}));
    assert((doc.GetSection("guest black list"s) == ini::Section{}));

    doc.AddSection("pets"s) = ini::Section{{"nasty"s, "rat"s}};
    assert(doc.GetSectionCount() == 6);
}