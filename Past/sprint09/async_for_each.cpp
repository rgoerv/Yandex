#include <algorithm>
#include <execution>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <future>
#include <functional>
#include <thread>
#include <iterator>

#include "log_duration.h"

using namespace std;

template <typename Strings>
void PrintStrings(const Strings& strings) {
    for (string_view s : strings) {
        cout << s << " ";
    }
    cout << endl;
}

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    for_each(std::execution::seq, range.begin(), range.end(), function);
}

template <typename ForwardRange, typename Function>
void AsyncForEach(ForwardRange& range, Function function) {

    auto size = range.size();
    auto count_tasks = thread::hardware_concurrency() - 1;

    vector<future<void>> results;

    auto chunk_size = 1;
    if (size > count_tasks) {
        chunk_size = (int)(size / count_tasks);
    }
    int64_t just_chunk_size = 0;

    auto chunk_begin = range.begin();
    auto chunk_end = range.end();
    for (auto it = range.begin(); it != range.end(); ++it) {

        if (just_chunk_size == chunk_size) {
            chunk_end = it;
            results.push_back(async([function, chunk_begin, chunk_end] { for_each(chunk_begin, chunk_end, function); }));
            just_chunk_size = 0;
            chunk_begin = it;
        }
        else {
            ++just_chunk_size;
        }
    }
    chunk_end = range.end();
    results.push_back(async([function, chunk_begin, chunk_end] { for_each(chunk_begin, chunk_end, function); }));

    for_each(results.begin(), results.end(), [](auto& getf) { getf.get(); });
}
template <typename ForwardRange, typename Function>
void ForEach(const std::execution::sequenced_policy&, ForwardRange& range, Function function) {
    ForEach(range, function);
}

template <typename ForwardRange, typename Function>
void ForEach(const std::execution::parallel_policy&, ForwardRange& range, Function function) {
    
    using this_iterator_categoty = typename iterator_traits<typename ForwardRange::iterator>::iterator_category;
    using random_access_it = random_access_iterator_tag;

    if constexpr (std::is_same_v<this_iterator_categoty, random_access_it>) {
        for_each(std::execution::par, range.begin(), range.end(), function);
    }
    else {
        AsyncForEach(range, function);
    }
}

int main() {
    auto reverser = [](string& s) { reverse(s.begin(), s.end()); };

    list<string> strings_list = { "cat", "dog", "code" };

    ForEach(strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    ForEach(execution::seq, strings_list, reverser);
    PrintStrings(strings_list);
    // cat dog code

    // единственный из вызовов, где должна работать ваша версия
    // из предыдущего задания
    ForEach(execution::par, strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    vector<string> strings_vector = { "cat", "dog", "code" };

    ForEach(strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    ForEach(execution::seq, strings_vector, reverser);
    PrintStrings(strings_vector);
    // cat dog code

    ForEach(execution::par, strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    return 0;
}