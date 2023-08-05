
void CheckYearValidity(int year) {
    if (year < 1) {
        throw domain_error("year is too small"s);
    }
    if (year > 9999) {
        throw domain_error("year is too big"s);
    }
}

void CheckMonthValidity(int month) {
    if (month < 1) {
        throw domain_error("month is too small"s);
    }
    if (month > 12) {
        throw domain_error("month is too big"s);
    }
}

void CheckDayValidity(const DateTime& dt) {
    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dt.day < 1) {
        throw domain_error("day is too small"s);
    }
    if (dt.day > month_lengths[dt.month - 1]) {
        throw domain_error("day is too big"s);
    }
}

void CheckHourValidity(int hour) {
    if (hour < 0) {
        throw domain_error("hour is too small"s);
    }
    if (hour > 23) {
        throw domain_error("hour is too big"s);
    }
}

void CheckMinuteValidity(int minute) {
    if (minute < 0) {
        throw domain_error("hour is too small"s);
    }
    if (minute > 23) {
        throw domain_error("hour is too big"s);
    }
}

void CheckSecondValidity(int second) {
    if (second < 0) {
        throw domain_error("hour is too small"s);
    }
    if (second > 23) {
        throw domain_error("hour is too big"s);
    }
}

void CheckDateTimeValidity(const DateTime& dt) {
    CheckYearValidity(dt.year);
    CheckMonthValidity(dt.month);
    CheckDayValidity(dt);
    CheckHourValidity(dt.hour);
    CheckMinuteValidity(dt.minute);
    CheckSecondValidity(dt.second);
}