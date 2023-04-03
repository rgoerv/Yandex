#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    // Реализуйте эту функцию
    //is.ignore();
    q.bus.clear();
    q.stop.clear();
    q.stops.clear();

    string line;
    while (getline(is, line)) {

        istringstream command(line);
        string TYPE;
        command >> TYPE;

//        if(TYPE.empty()) { cout << "EMPTY!" << endl; }

        if(TYPE == "NEW_BUS") {
            q.type = QueryType::NewBus;
            int count_stop;
            command >> q.bus >> count_stop;
            for(size_t i = 0; i < count_stop; ++i) {
                string stop;
                command >> stop;
                q.stops.push_back(stop);
            }
            break;
        }
        else if(TYPE == "BUSES_FOR_STOP") {
            q.type = QueryType::BusesForStop;
            command >> q.stop;
            break;
        }
        else if(TYPE == "STOPS_FOR_BUS") {
            q.type = QueryType::StopsForBus;
            command >> q.bus;
            break;
        }
        else if(TYPE == "ALL_BUSES") {
            q.type = QueryType::AllBuses;
            break;
        }
        else { 
            break;
        }
    }
    return is;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
    vector<string> buses_to_stop;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
    if (r.buses_to_stop.empty()) {
        os << "No stop"s << endl;;
    }
    else{
        for(const auto & bus : r.buses_to_stop)
        {
            os << bus << " "s;
        }
        os << endl;
    }
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    string bus;
    vector<string> stops_for_this_bus;
    map<string, vector<string>> stops_to_buses;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    if(r.stops_for_this_bus.empty()) {
        os << "No bus"s << endl;
    }
    else{

    for (const string& stop : r.stops_for_this_bus) {

        os << "Stop "s << stop << ": "s;

        if(!r.stops_to_buses.count(stop)) {
            continue;
        }
        else if (r.stops_to_buses.at(stop).size() == 1) {
            os << "no interchange"s;
            os << endl;
        }
        else {
            for (const string& other_bus : r.stops_to_buses.at(stop)) {
                if (r.bus != other_bus) {
                    os << other_bus << " "s;
                }
            }
            os << endl;
        }
    }

    }


    return os;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
    map<string, vector<string>> buses_to_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    if (r.buses_to_stops.empty()) 
    {
        os << "No buses"s;
        os << endl;
    } 
    else 
    {
        for (const auto& bus_item : r.buses_to_stops) 
        {
            os << "Bus "s << bus_item.first << ": "s;
            for (const string& stop : bus_item.second) 
            {
                os << stop << " "s;
            }
            os << endl;
        }
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        // Реализуйте этот метод
        ALL_bus_to_stops[bus] = stops;
        for(const auto & stop : stops)
        {
            ALL_stop_to_buses[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        // Реализуйте этот метод
        BusesForStopResponse result;
        result.buses_to_stop = {};
        if(ALL_stop_to_buses.count(stop)) {
            for(const auto & bus : ALL_stop_to_buses.at(stop)) {
                result.buses_to_stop.push_back(bus);
            }
        }
        return result;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        // Реализуйте этот метод
        StopsForBusResponse result;
        result.bus = bus;
        if(ALL_bus_to_stops.count(bus)) {
            result.stops_for_this_bus = ALL_bus_to_stops.at(bus);
        }
        
        for(const auto & stop_to_bus : ALL_stop_to_buses){
            if(count(stop_to_bus.second.begin(), stop_to_bus.second.end(), bus) != 0){
                result.stops_to_buses.insert(stop_to_bus);
            }
        }

        return result;
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        AllBusesResponse result;
        result.buses_to_stops = ALL_bus_to_stops;
        
        return result;
    }

    map<string, vector<string>> GetALL_bus_to_stops() const {
        return ALL_bus_to_stops;
    }

    map<string, vector<string>> GetALL_stop_to_buses() const {
        return ALL_stop_to_buses;
    }

private:
    map<string, vector<string>> ALL_bus_to_stops = {}, ALL_stop_to_buses = {};
};

// Реализуйте функции и классы, объявленные выше, чтобы эта функция main
// решала задачу "Автобусные остановки"

void TestInputQuery(){

    istringstream input;
    input.str("NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo"s);
    Query q;
    input >> q;
    assert(q.type == QueryType::NewBus);
    assert(q.bus == "32"s);
    vector<string> stops = {{"Tolstopaltsevo"s}, {"Marushkino"s}, {"Vnukovo"s}};
    assert(q.stops == stops);
    input.clear();

    input.str("BUSES_FOR_STOP Vnukovo"s);
    Query p;
    input >> p;
    assert(p.type == QueryType::BusesForStop);
    assert(p.stop == "Vnukovo"s);
    input.clear();

    input.str("STOPS_FOR_BUS 32K"s);
    Query r;
    input >> r;
    assert(r.type == QueryType::StopsForBus);
    assert(r.bus == "32K"s);
    input.clear();

    input.str("ALL_BUSES");
    Query l;
    input >> l;
    assert(l.type == QueryType::AllBuses);
    input.clear();
}

void TestOutput(){

    BusesForStopResponse q;
    q.buses_to_stop = {"32"s, "32K"s, "50"s};
    ostringstream output_q;
    output_q << q;
    assert(output_q.str() == "32 32K 50 \n"s);
    output_q.clear();


    StopsForBusResponse p;
    p.bus = "272"s;
    p.stops_for_this_bus = { {"Vnukovo"s}, {"Moskovsky"s}, {"Rumyantsevo"s}, {"Troparyovo"s}};
    p.stops_to_buses = { 
        { {"Vnukovo"}, { {"32"s}, {"32K"s}, {"950"s},  {"272"s}}}, 
        { {"Moskovsky"}, { {"272"s}}},
        { {"Rumyantsevo"}, { {"272"s}}},
        { {"Troparyovo"}, { {"950"s, {"272"s}}}}
    };

    ostringstream output_p;
    output_p << p;
    assert(output_p.str() == "Stop Vnukovo: 32 32K 950 \nStop Moskovsky: no interchange\nStop Rumyantsevo: no interchange\nStop Troparyovo: 950 \n"s);
    output_p.clear();

    AllBusesResponse r;
    r.buses_to_stops = { { "272"s, { { "Vnukovo"s}, { "Moskovsky"s}, { "Rumyantsevo"s}, { "Troparyovo"s}}},
    {"32"s, { { "Tolstopaltsevo"s}, { "Marushkino"s}, { "Vnukovo"s}}}
    };

    ostringstream output_r;
    output_r << r;
    assert(output_r.str() == "Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo \nBus 32: Tolstopaltsevo Marushkino Vnukovo \n"s);
    output_r.clear();
}

void TestAddBus(){
    BusManager bm;
    const string bus = "32"s;
    const vector<string> stops = { "Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s};
    bm.AddBus(bus, stops);

    map<string, vector<string>> ALL_bus_to_stops = {{bus, stops}};
    map<string, vector<string>> ALL_stop_to_buses = { {"Tolstopaltsevo"s, {"32"s}}, {"Marushkino"s, {"32"s}}, {"Vnukovo"s, {"32"s}}};   
    assert(bm.GetALL_bus_to_stops() == ALL_bus_to_stops);
    assert(bm.GetALL_stop_to_buses() == ALL_stop_to_buses);
}

void TestGetBusesForStop(){
    BusManager bm;
    bm.AddBus("32"s, { "Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
    bm.AddBus("32K"s, { "Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s});

    BusesForStopResponse r;
    r.buses_to_stop = { "32"s, "32K"s};
    
    assert(bm.GetBusesForStop("Vnukovo"s).buses_to_stop == r.buses_to_stop);
}

void TestGetStopsForBus(){
    BusManager bm;
    bm.AddBus("32"s, { "Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
    bm.AddBus("32K"s, { "Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
    bm.AddBus("950"s, { "Kokoshkino"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Troparyovo"s,});
    bm.AddBus("272"s, { "Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s});

    StopsForBusResponse p;
    p.bus = "272"s;
    p.stops_for_this_bus = { {"Vnukovo"s}, {"Moskovsky"s}, {"Rumyantsevo"s}, {"Troparyovo"s}};
    p.stops_to_buses = { 
        { {"Vnukovo"}, { {"32"s}, {"32K"s}, {"950"s},  {"272"s}}}, 
        { {"Moskovsky"}, { {"272"s}}},
        { {"Rumyantsevo"}, { {"272"s}}},
        { {"Troparyovo"}, { {"950"s, {"272"s}}}}
    };

    StopsForBusResponse q = bm.GetStopsForBus("272"s);
    assert(q.bus == p.bus);
    assert(q.stops_for_this_bus == p.stops_for_this_bus);
    assert(q.stops_to_buses == p.stops_to_buses);
}

void TestGetAllBuses(){
    BusManager bm;
    bm.AddBus("32"s, { "Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
    bm.AddBus("32K"s, { "Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
    bm.AddBus("950"s, { "Kokoshkino"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Troparyovo"s,});
    bm.AddBus("272"s, { "Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s});

    assert(bm.GetAllBuses().buses_to_stops == bm.GetALL_bus_to_stops());
}

void TEST(){
    TestInputQuery();
    TestOutput();
    TestAddBus();
    TestGetBusesForStop();
    TestGetStopsForBus();
    TestGetAllBuses();
}

int main() {
    TEST();

    int query_count;
    Query q;

    cin >> query_count;
    cin.ignore();

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop);
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus);
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses();
                break;
        }
    }
}