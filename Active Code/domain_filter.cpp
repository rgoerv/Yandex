#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(string domain) 
        : domain_(move(domain))
    {
        domain_.insert(0, 1, '.');
        reverse_domain_ = domain_;
        reverse(reverse_domain_.begin(), reverse_domain_.end()); 
    }

    bool operator==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool operator<(const Domain& other) const {
        return lexicographical_compare(reverse_domain_.begin(), reverse_domain_.end(), 
                            other.reverse_domain_.begin(), other.reverse_domain_.end());
    }

    // метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const {
        if (reverse_domain_.find(other.reverse_domain_) == 0) {
            return true;
        }
        return false;
    }

private:
    string domain_;
    string reverse_domain_;
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename InputIt>
    DomainChecker(InputIt begin, InputIt end) 
        : forbidden_domains_(begin, end)
    {
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        auto unique_end = unique(forbidden_domains_.begin(), forbidden_domains_.end(), 
            [](const Domain& lhs, const Domain& rhs){
                return lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs);
            });
        forbidden_domains_.erase(unique_end, forbidden_domains_.end());
    }
    
    // метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if(it != forbidden_domains_.begin())
        {
            if(domain.IsSubdomain(*prev(it))) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<Domain> forbidden_domains_;
};

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

// функция ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Number>
std::vector<Domain> ReadDomains(istream& input, Number number) {
    std::vector<Domain> result;
    string domain;

    for(int i = number; i > 0; --i) {
        getline(input, domain);
        result.push_back(Domain{domain});
    }

    return result;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}