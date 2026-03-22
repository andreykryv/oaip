#include "Date.h"
#include <cstdlib> 

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }



bool Date::IsLeap() const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}





static int daysInMonth(int month, int year) {
    if (month == 2) {
        return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    return 31;
}





Date Date::NextDay() const {
    int d = day + 1;
    int m = month;
    int y = year;
    if (d > daysInMonth(m, y)) {
        d = 1;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }
    return Date(d, m, y);
}

Date Date::PreviousDay() const {
    int d = day - 1;
    int m = month;
    int y = year;
    if (d < 1) {
        m--;
        if (m < 1) {
            m = 12;
            y--;
        }
        d = daysInMonth(m, y);
    }
    return Date(d, m, y);
}




static int dayOfYear(const Date& date) {
    int days = 0;
    for (int m = 1; m < date.getMonth(); ++m) {
        days += daysInMonth(m, date.getYear());
    }
    days += date.getDay();
    return days;
}

short Date::WeekNumber() const {
    int doy = dayOfYear(*this);
    return (doy - 1) / 7 + 1;  
}

static long long dateToDays(const Date& date) {
    long long total = 0;
    int y = date.getYear();
    int m = date.getMonth();
    int d = date.getDay();

    for (int year = 1; year < y; ++year) {
        total += ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 366 : 365;
    }
    for (int month = 1; month < m; ++month) {
        total += daysInMonth(month, y);
    }
    total += d;
    return total;
}

int Date::Duration(const Date& other) const {
    long long daysThis = dateToDays(*this);
    long long daysOther = dateToDays(other);
    return static_cast<int>(llabs(daysOther - daysThis));
}

int Date::DaysTillYourBithday(const Date& birthdaydate) const {
    Date bdThisYear(birthdaydate.getDay(), birthdaydate.getMonth(), year);
    int doyCurrent = dayOfYear(*this);
    int doyBD = dayOfYear(bdThisYear);
    if (doyBD <= doyCurrent) {
        bdThisYear = Date(birthdaydate.getDay(), birthdaydate.getMonth(), year + 1);
    }
    return this->Duration(bdThisYear);
}