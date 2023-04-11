#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <functional>
#include <tuple>

using namespace std;

template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    unordered_set<size_t> hashs;
    unordered_set<string> words;
    size_t count_collisions = 0;
    string word;
    while (text >> word) {
        if(words.count(word) > 0) {
            continue;
        }
        size_t hash_ = hasher(word);
        if(hashs.count(hash_) > 0) {
            ++count_collisions;
        }
        hashs.insert(hash_);
        words.insert(word);
    }
    return count_collisions;
}

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct DumbbellHash {
    static const uint64_t N = 37;
    size_t operator() (const Dumbbell& f) const {
        Circle circle_one = f.circle1;
        hash<double> {} (circle_one.x);
        Circle circle_too = f.circle2;
        return static_cast<size_t>(hash<double> {} (circle_one.x) + N * 1 * hash<double> {} (circle_one.y) 
            + N * 2 *hash<double> {} (circle_one.r) 
            + N * 3 *hash<double> {} (circle_too.x) + N * 4 *hash<double> {} (circle_too.y) 
            + N * 5 *hash<double> {} (circle_too.r)
            + N * 6 *hash<string> {} (f.text));
    }
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{{10, 11.5, 2.3}, {3.14, 15, -8}, "abc"s};
    cout << "Dumbbell hash "s << hash(dumbbell);
}