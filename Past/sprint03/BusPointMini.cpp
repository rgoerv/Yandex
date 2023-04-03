#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    int q;
    cin >> q;

    map<string, vector<string>> buses_to_stops, stops_to_buses;

    for (int i = 0; i < q; ++i) {
        string operation_code;
        cin >> operation_code;

        if (operation_code == "NEW_BUS"s) {
            string bus;
            cin >> bus;
            int stop_count;
            cin >> stop_count;
            vector<string>& stops = buses_to_stops[bus];
            stops.resize(stop_count);
            for (string& stop : stops) {
                cin >> stop;
                stops_to_buses[stop].push_back(bus);
            }

        } else if (operation_code == "BUSES_FOR_STOP"s) {
            string stop;
            cin >> stop;
            if (stops_to_buses.count(stop) == 0) {
                cout << "No stop"s << endl;
            } else {
                for (const string& bus : stops_to_buses[stop]) {
                    cout << bus << " "s;
                }
                cout << endl;
            }

        } else if (operation_code == "STOPS_FOR_BUS"s) {
            string bus;
            cin >> bus;
            if (buses_to_stops.count(bus) == 0) {
                cout << "No bus"s << endl;
            } else {
                for (const string& stop : buses_to_stops[bus]) {
                    cout << "Stop "s << stop << ": "s;
                    if (stops_to_buses[stop].size() == 1) {
                        cout << "no interchange"s;
                    } else {
                        for (const string& other_bus : stops_to_buses[stop]) {
                            if (bus != other_bus) {
                                cout << other_bus << " "s;
                            }
                        }
                    }
                    cout << endl;
                }
            }

        } else if (operation_code == "ALL_BUSES"s) {
            if (buses_to_stops.empty()) {
                cout << "No buses"s << endl;
            } else {
                for (const auto& bus_item : buses_to_stops) {
                    cout << "Bus "s << bus_item.first << ": "s;
                    for (const string& stop : bus_item.second) {
                        cout << stop << " "s;
                    }
                    cout << endl;
                }
            }
        }
    }

    return 0;
}