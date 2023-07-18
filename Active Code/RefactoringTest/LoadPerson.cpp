#include <string_view>
#include <vector>

struct DBConfig {
    string_view name;
    int connection_timeout;
    bool allow_exceptions;
    DBLogLevel log_level;

    DBConfig& SetName(string_view name) {
        DBConfig::name = name;
        return *this;
    }
    DBConfig& SetConnectionTimeout(int connection_timeout) {
        DBConfig::connection_timeout = connection_timeout;
        return *this;
    }
    DBConfig& SetAllowExceptions(bool allow_exceptions) {
        DBConfig::allow_exceptions = allow_exceptions;
        return *this;
    }
    DBConfig& SetName(DBLogLevel log_level) {
        DBConfig::log_level = log_level;
        return *this;
    }
};

vector<Person> LoadPersons(DBConfig db_config, int min_age, int max_age, string_view name_filter) {
    DBConnector connector(db_config.allow_exceptions, db_config.log_level);
    DBHandler db;
    if (db_config.name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_config.name, db_config.connection_timeout);
    } else {
        db = connector.Connect(db_config.name, db_config.connection_timeout);
    }
    if (!db_config.allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}