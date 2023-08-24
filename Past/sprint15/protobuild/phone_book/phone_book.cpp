#include "phone_book.h"
#include "iterator_range.h"

#include <contact.pb.h>

#include <algorithm>
#include <string_view>
#include <vector>
#include <iostream>

using namespace std;

PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {

    if(name_prefix.empty()) {
        return ContactRange{ contacts_.begin(), contacts_.end() };
    }

    std::sort(contacts_.begin(), contacts_.end(), [](const Contact& lhs, const Contact& rhs){
        return lhs.name < rhs.name;
    });

    auto lw = [](const Contact& contact, std::string_view name){
        return contact.name < name;
    };

    auto ub = [](std::string_view name, const Contact& contact){
        return contact.name.substr(0, name.length()) > name;
    };

    auto begin = std::lower_bound(contacts_.begin(), contacts_.end(), name_prefix, lw);

    return ContactRange(begin, std::upper_bound(begin, contacts_.end(), name_prefix, ub));
}

void PhoneBook::SaveTo(std::ostream& output) const {
    
    phone_book_serialize::ContactList list;

    for(const auto& contact : contacts_) {
        phone_book_serialize::Date date;

        phone_book_serialize::Contact pb_contact;
        pb_contact.set_name(contact.name);

        if(contact.birthday.has_value()) {
            date.set_day(contact.birthday.value().day);
            date.set_month(contact.birthday.value().month);
            date.set_year(contact.birthday.value().year);
            *pb_contact.mutable_birthday() = date;
        }

        for(const auto& number : contact.phones) {
            pb_contact.add_phone_number(number);
        }
        
        *list.add_contact() = pb_contact;
    }
    list.SerializePartialToOstream(&output);
}

PhoneBook DeserializePhoneBook(std::istream& input) {
    phone_book_serialize::ContactList pb_list;

    if (!pb_list.ParseFromIstream(&input)) {
        return PhoneBook{std::vector<Contact>{}};
    }

    std::vector<Contact> contacts;
    contacts.reserve(pb_list.contact_size());

    for(auto it = 0; it < pb_list.contact_size(); ++it) {
        Contact contact;
        contact.name = pb_list.contact(it).name();
        
        if(pb_list.contact(it).has_birthday()) {
            Date date;
            date.day = pb_list.contact(it).birthday().day();
            date.month =  pb_list.contact(it).birthday().month();
            date.year = pb_list.contact(it).birthday().year();

            contact.birthday = std::optional<Date>(date);
        }

        std::vector<std::string> phones;
        phones.reserve(pb_list.contact(it).phone_number_size());

        for(auto phone = 0; phone < phones.capacity(); ++phone) {
            phones.push_back(pb_list.contact(it).phone_number(phone));
        }
        contact.phones = phones;
        
        contacts.push_back(contact);
    }

    return PhoneBook{std::move(contacts)};;
}