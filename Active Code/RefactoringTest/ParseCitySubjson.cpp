class CountryConfig {
    CountryConfig& SetName(string name) {
        CountryConfig::name = std::move(name);
        return *this;
    }
    CountryConfig& SetIsoCode(string iso_code) {
        CountryConfig::iso_code = std::move(iso_code);
        return *this;
    }
    CountryConfig& SetPhoneCode(string phone_code) {
        CountryConfig::phone_code = std::move(phone_code);
        return *this;
    }
    CountryConfig& SetTimeZone(string time_zone) {
        CountryConfig::time_zone = std::move(time_zone);
        return *this;
    }
    CountryConfig& Build() {
        return *this;
    }
private:
    string name;
    string iso_code;
    string phone_code;
    string time_zone;
};



// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const CountryConfig& config,
                      const vector<Language>& languages) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country_phone_code + city_obj["phone_code"s].AsString(), country_name, country_iso_code,
                          country_time_zone, languages});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson(cities, country_obj["cities"s], country.name, country.iso_code, country.phone_code,
                         country.time_zone, country.languages);
    }
}