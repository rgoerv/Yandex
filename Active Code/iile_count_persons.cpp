#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <functional>
#include <set>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
        : first(begin)
        , last(end) {
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

private:
    Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
    return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
    string name;
    int age, income;
    bool is_male;
};

bool operator==(const Person& lhs, const Person& rhs) {
    return lhs.name == rhs.name &&
            lhs.age == rhs.age &&
            lhs.income == rhs.income &&
            lhs.is_male == rhs.is_male;
}

vector<Person> ReadPeople(istream& input) {
    int count;
    input >> count;

    vector<Person> result(count);
    for (Person& p : result) {
        char gender;
        input >> p.name >> p.age >> p.income >> gender;
        p.is_male = gender == 'M';
    }

    return result;
}

int main() {
    const vector<Person> people = std::invoke([]{
        vector<Person> people_buf = ReadPeople(cin);
        sort(people_buf.begin(), people_buf.end(), 
                [](const Person& lhs, const Person& rhs) {
                return std::make_tuple(lhs.name, lhs.is_male, lhs.age, lhs.income) 
                < std::make_tuple(rhs.name, rhs.is_male, rhs.age, rhs.income);
            });
        // auto it = unique(people_buf.begin(), people_buf.end());
        // people_buf.erase(it, people_buf.end());
        return people_buf;
    });

    for (string command; cin >> command;) {
        if (command == "AGE"s) {
            int adult_age;
            cin >> adult_age;

            const vector<Person> people_age = std::invoke([&people]{
                vector<Person> people_age(people);
                sort(people_age.begin(), people_age.end(), 
    	            [](const Person& lhs, const Person& rhs) {
                    return lhs.age < rhs.age;
                });
                return people_age;
            });

            auto adult_begin = lower_bound(people_age.begin(), people_age.end(), adult_age, 
            	[](const Person& lhs, int age) {
                return lhs.age < age;
            });

            cout << "There are "s << distance(adult_begin, people_age.end()) << " adult people for maturity age "s
                 << adult_age << '\n';
        } else if (command == "WEALTHY"s) 
        {
            int count;
            cin >> count;

            vector<Person> people_wealthy(people);

            auto head = Head(people_wealthy, count);

            partial_sort(head.begin(), head.end(), people_wealthy.end(),
            	[](const Person& lhs, const Person& rhs) {
                return lhs.income > rhs.income;
            });

            int total_income = accumulate(head.begin(), head.end(), 0, [](int cur, Person& p) {
                return p.income += cur;
            });
            cout << "Top-"s << count << " people have total income "s << total_income << '\n';
        } else if (command == "POPULAR_NAME"s) 
        {
            char gender;
            cin >> gender;

            vector<Person> people_name(people);

            IteratorRange range{people_name.begin(), partition(people_name.begin(), people_name.end(), 
            					[&gender](const Person& p) {
                                    return p.is_male == (gender == 'M');
                                })};
            if (range.begin() == range.end()) {
                cout << "No people of gender "s << gender << '\n';
            } else {
                sort(range.begin(), range.end(), 
                	[](const Person& lhs, const Person& rhs) {
                    return lhs.name < rhs.name;
                });
                const string* most_popular_name = &range.begin()->name;
                int count = 1;
                for (auto i = range.begin(); i != range.end();) {
                    auto same_name_end = find_if_not(i, range.end(), 
                    	[&i](const Person& p) {
                        return p.name == i->name;
                    });
                    auto cur_name_count = distance(i, same_name_end);
                    if (cur_name_count > count) {
                        count = cur_name_count;
                        most_popular_name = &i->name;
                    }
                    i = same_name_end;
                }
                cout << "Most popular name among people of gender "s << gender << " is "s << *most_popular_name << '\n';
            }
        }
    }
}