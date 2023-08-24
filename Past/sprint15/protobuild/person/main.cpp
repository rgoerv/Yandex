#include <person.pb.h>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string_view>
#include <optional>

using namespace std;

person::Person CreatePerson() {
    person::Person p;

    // Зададим имя
    p.set_name("Chelovek"s);
    *p.mutable_name() += " Rasseyaniy"sv;

    // Добавим пару номеров телефона
    p.add_phone_number("33-22-22-33-22");
    p.add_phone_number("+7-123-456-789");

    // Создадим объект адреса
    person::Address addr;
    addr.set_street("Basseynaya"s);
    addr.set_building(1);

    // Установим адрес
    *p.mutable_address() = move(addr);

    return p;
}

void Serialize(const filesystem::path& path, 
               const person::Person& object) {    
    ofstream out_file(path, ios::binary);
    object.SerializeToOstream(&out_file);
}

optional<person::Person> DeserializePerson(const filesystem::path& path) {
    ifstream in_file(path, ios::binary);
    person::Person object;
    if (!object.ParseFromIstream(&in_file)) {
        return nullopt;
    }

    // тут нужен move, поскольку возвращается другой тип
    return {move(object)};
}

int main() {
    const filesystem::path path = "test_file.bin"sv;

    Serialize(path, CreatePerson());

    optional<person::Person> deserialized = DeserializePerson(path);

    if (!deserialized) {
        cout << "Ne poluchilos' prochitat' messange"sv << endl;
    }
    else {
        cout << deserialized->name();
        if (deserialized->has_address()) {
             cout << " s ulitsy "sv 
                  << deserialized->address().street();
        }
        cout << endl;
    }
}