#include <iostream>
#include <set>
#include <functional>

using namespace std;

struct Book {
    Book() = delete;

    Book(int client_id_, string hotel_name_, int64_t time_, int room_count_)
        : client_id(client_id_)
        , hotel_name(hotel_name_)
        , time(time_)
        , room_count(room_count_)
    {
    }

    int client_id = 0;
    string hotel_name;
    int64_t time = 0;
    int room_count = 0;
};

struct BookHasher {
    size_t operator()(const Book& book) {
        return std::hash<int> {} (book.client_id) +
            std::hash<string> {} (book.hotel_name) * 37 +
            std::hash<int64_t> {} (book.time) * 37 * 2 + 
            std::hash<int> {} (book.room_count) * 37 * 3;
    }
};

class HotelManager {
public:
    void Book(const Book& book) {
        clients.insert(book);
    }

    int ComputeClientCount(string_view hotel_name) {
        int result = 0;

        for(auto it = clients.begin(); it != clients.end(); ) {
            
        } 

    }

    int ComputeRoomCount(string_view hotel_name) {

    }

private:
    set<::Book, std::less<>> clients;
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
            manager.Book(Book {client_id, hotel_name, time, room_count});
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

    return 0;
}