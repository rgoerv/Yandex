#include <iostream>
#include <deque>
#include <functional>
#include <map>
#include <set>

using namespace std;

struct Book {
    Book() = delete;

    Book(int client_id_, int64_t time_, int room_count_)
        : client_id(client_id_)
        , time(time_)
        , room_count(room_count_)
    {
    }

    int client_id = 0;
    int64_t time = 0;
    int room_count = 0;
};

struct BookHasher {
    size_t operator()(const Book& book) {
        return std::hash<int> {} (book.client_id)  +
            std::hash<int64_t> {} (book.time) * 37 * 2 + 
            std::hash<int> {} (book.room_count) * 37 * 3;
    }
};

class HotelManager {
public:
    void Book(const Book& book, const string& hotel_name_) {
        hotel_to_book[hotel_name_].push_back(book);
        curr_time_ = book.time;
    }

    int ComputeClientCount(string_view hotel_name) const {

        if(hotel_to_book.count(hotel_name) == 0) {
            return 0;
        }
        
        set<int> clients;

        const auto& books = hotel_to_book.at(static_cast<string>(hotel_name));
        const auto& minimum_time = curr_time_ - 86400;

        for(const auto& book : books) {
            if(book.time > minimum_time && book.time <= curr_time_) {
                clients.insert(book.client_id);
            }
        }
        return static_cast<int>(clients.size());
    }

    int ComputeRoomCount(string_view hotel_name) const {
        if(hotel_to_book.count(hotel_name) == 0) {
            return 0;
        }
        
        int result = 0;

        const auto& books = hotel_to_book.at(static_cast<string>(hotel_name));
        const auto& minimum_time = curr_time_ - 86400;

        for(const auto& book : books) {
            if(book.time > minimum_time && book.time <= curr_time_) {
                result += book.room_count;
            }
        }
        return result;
    }

private:
    map<string, deque<::Book>, std::less<>> hotel_to_book;
    int64_t curr_time_ = 0;
};

int main() {
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(Book {client_id, time, room_count}, hotel_name);
        } else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            } else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }
    // system("pause");
    return 0;
}