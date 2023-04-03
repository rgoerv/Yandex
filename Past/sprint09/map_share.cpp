#include <algorithm>
#include <cstdlib>
#include <future>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <mutex>

#include "log_duration.h"
#include "test_framework.h"

using namespace std::string_literals;
using std::mutex;
using std::lock_guard;
using std::map;
using std::vector;
using std::atomic;

template <typename Key, typename Value>
class ConcurrentMap {
public:
    static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys");

    struct Access {
        Access(mutex& value_mutex, Value& value) : access(value_mutex), ref_to_value(value) {
        }
        lock_guard<mutex> access;
        Value& ref_to_value;
    };

    ConcurrentMap() = default;

    explicit ConcurrentMap(size_t bucket_count) : map_mutexs_(bucket_count), bucket_maps_(bucket_count) {
    }

    Access operator[](const Key& key) {
        uint64_t uKey = static_cast<uint64_t>(key);
        uint64_t position_in_vector = uKey % bucket_maps_.size();

        return Access(map_mutexs_[position_in_vector], bucket_maps_[position_in_vector][static_cast<Key>(uKey)]);
    }

    std::map<Key, Value> BuildOrdinaryMap() {        
        map<Key, Value> OrdinaryMap;
        size_t mutex_index = 0;

        for (const auto& solo_map : bucket_maps_) {
            lock_guard guard(map_mutexs_[mutex_index++]);
            OrdinaryMap.insert(solo_map.begin(), solo_map.end());
        }
        return OrdinaryMap;
    }

private:
    vector<mutex> map_mutexs_;
    vector<map<Key, Value>> bucket_maps_;
};

using namespace std;

void RunConcurrentUpdates(ConcurrentMap<int, int>& cm, size_t thread_count, int key_count) {
    auto kernel = [&cm, key_count](int seed) {
        vector<int> updates(key_count);
        iota(begin(updates), end(updates), -key_count / 2);
        shuffle(begin(updates), end(updates), mt19937(seed));

        for (int i = 0; i < 2; ++i) {
            for (auto key : updates) {
                ++cm[key].ref_to_value;
            }
        }
    };

    vector<future<void>> futures;
    for (size_t i = 0; i < thread_count; ++i) {
        futures.push_back(async(kernel, (int)i));
    }
}

void TestConcurrentUpdate() {
    constexpr size_t THREAD_COUNT = 3;
    constexpr size_t KEY_COUNT = 50000;

    ConcurrentMap<int, int> cm(THREAD_COUNT);
    RunConcurrentUpdates(cm, THREAD_COUNT, KEY_COUNT);

    const auto result = cm.BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), KEY_COUNT);
    for (auto& [k, v] : result) {
        AssertEqual(v, 6, "Key = " + to_string(k));
    }
}

void TestReadAndWrite() {
    ConcurrentMap<size_t, string> cm(5);

    auto updater = [&cm] {
        for (size_t i = 0; i < 50000; ++i) {
            cm[i].ref_to_value.push_back('a');
        }
    };
    auto reader = [&cm] {
        vector<string> result(50000);
        for (size_t i = 0; i < result.size(); ++i) {
            result[i] = cm[i].ref_to_value;
        }
        return result;
    };

    auto u1 = async(updater);
    auto r1 = async(reader);
    auto u2 = async(updater);
    auto r2 = async(reader);

    u1.get();
    u2.get();

    for (auto f : { &r1, &r2 }) {
        auto result = f->get();
        ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
            return s.empty() || s == "a" || s == "aa";
            }));
    }
}

void TestSpeedup() {
    {
        ConcurrentMap<int, int> single_lock(1);

        LOG_DURATION("Single lock"s);
        RunConcurrentUpdates(single_lock, 4, 50000);
    }
    {
        ConcurrentMap<int, int> many_locks(100);

        LOG_DURATION("100 locks"s);
        RunConcurrentUpdates(many_locks, 4, 50000);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestReadAndWrite);
    RUN_TEST(tr, TestSpeedup);
}