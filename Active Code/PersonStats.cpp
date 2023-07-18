#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Gender { FEMALE, MALE };

struct Person {
    int age;           // возраст
    Gender gender;     // пол
    bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = range_copy.begin() + range_copy.size() / 2;
    nth_element(range_copy.begin(), middle, range_copy.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
    });
    return middle->age;
}

// напишите сигнатуру и реализацию функции PrintStats
void PrintStats(vector<Person> persons) {
    cout << "Median age = "s << ComputeMedianAge(persons.begin(), persons.end()) << endl;

    vector<Person> separated_gender(persons.begin(), persons.end());

    sort(separated_gender.begin(), separated_gender.end(), [](const Person lhs, const Person rhs){
        return lhs.gender != rhs.gender && lhs.gender == Gender::FEMALE;
    });

    auto first_male = find_if(separated_gender.begin(), separated_gender.end(), [](const Person person){
        return person.gender == Gender::MALE;
    });

    cout << "Median age for females = "s << ComputeMedianAge(separated_gender.begin(), first_male) << endl;
    cout << "Median age for males = "s << ComputeMedianAge(first_male, separated_gender.end()) << endl;

    vector<Person> fem_separated_employ(separated_gender.begin(), first_male);

    sort(fem_separated_employ.begin(), fem_separated_employ.end(), [](const Person lhs, const Person rhs){
        return lhs.is_employed != rhs.is_employed && lhs.is_employed;
    });

    auto first_unemploy = find_if(fem_separated_employ.begin(), fem_separated_employ.end(), [](const Person person) {
        return !person.is_employed;
    });

    cout << "Median age for employed females = "s << ComputeMedianAge(fem_separated_employ.begin(), first_unemploy) << endl;
    cout << "Median age for unemployed females = "s << ComputeMedianAge(first_unemploy, fem_separated_employ.end()) << endl;

    vector<Person> male_separated_employ(first_male, separated_gender.end());

    sort(male_separated_employ.begin(), male_separated_employ.end(), [](const Person lhs, const Person rhs){
        return lhs.is_employed != rhs.is_employed && lhs.is_employed;
    });

    auto first_unemploy_man = find_if(male_separated_employ.begin(), male_separated_employ.end(), [](const Person person) {
        return !person.is_employed;
    });

    cout << "Median age for employed males = " << ComputeMedianAge(male_separated_employ.begin(), first_unemploy_man) << endl;
    cout << "Median age for unemployed males = "s << ComputeMedianAge(first_unemploy_man, male_separated_employ.end())  << endl;
}

int main() {
    vector<Person> persons = {
        {31, Gender::MALE, false},   {40, Gender::FEMALE, true},  {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},  {80, Gender::FEMALE, false}, {78, Gender::MALE, false},
        {10, Gender::FEMALE, false}, {55, Gender::MALE, true},
    };
    PrintStats(persons);
}