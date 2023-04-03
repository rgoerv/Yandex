#include <iostream>
#include <vector>

using namespace std;

void PrintBirthday(string name, int birth_year, int age)
{
    cout << name << " will turn " << age << " in " << birth_year + age << endl;
    
    if(age % 10 == 0) cout << "Happy anniversary!" << endl;
    else if(age % 50 == 0) cout << "Happy great anniversary!" << endl;
    else cout << "Happy birthday!" << endl;
}

int main()
{
    PrintBirthday("George"s, 1788, 10);
    PrintBirthday("William"s, 1564, 100);
    PrintBirthday("Percy"s, 1792, 41);
}
