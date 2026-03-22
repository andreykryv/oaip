#include "Date.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>


//  Constructor

Date::Date() : m_day(1), m_month(1), m_year(1) {}

Date::Date(int day, int month, int year)
    : m_day(day), m_month(month), m_year(year)
{
    if (!isValid())
        throw std::invalid_argument("Invalid date: " + toString());
}


//  Get

int Date::day() const { return m_day;}
int Date::month() const { return m_month;}
int Date::year() const { return m_year;}


bool Date::isLeap() const//высокосный?
{
    int y = m_year;
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}


int Date::daysInMonth(int month, int year) const
{
    switch (month) {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: {
            bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            return leap ? 29 : 28;
        }
        default: throw std::invalid_argument("Invalid month");
    }
}


bool Date::isValid() const//проверка года
{
    if (m_year < 1) return false;
    if (m_month < 1 || m_month > 12) return false;
    if (m_day < 1 || m_day > daysInMonth(m_month, m_year)) return false;
    return true;
}


Date Date::nextDay() const
{int d = m_day + 1;
    int m = m_month;
    int y = m_year;
    if (d > daysInMonth(m, y)) {
        d = 1;
        if (++m > 12) { m = 1; ++y; }
    }
    return Date(d, m, y);
}

Date Date::previousDay() const
{ int d = m_day - 1;
    int m = m_month;
    int y = m_year;
    if (d < 1) {
        if (--m < 1) { m = 12; --y; }
        if (y < 1) throw std::out_of_range("No day before 01.01.0001");
        d = daysInMonth(m, y);
    }
    return Date(d, m, y);
}


long Date::toDays() const
{
    long y = m_year - 1;
    long days = y * 365 + y / 4 - y / 100 + y / 400;
    for (int mo = 1; mo < m_month; ++mo)
        days += daysInMonth(mo, m_year);
    days += m_day;
                  return days;
}

Date Date::fromDays(long days)
{
   
    int y = static_cast<int>(days / 365.2425) + 1;
    Date tmp(1, 1, std::max(1, y));
    while (tmp.toDays() > days) { --y; tmp = Date(1,1,std::max(1,y)); }
    while (Date(1,1,y+1).toDays() <= days) ++y;
    Date d(1,1,y);
    long rem = days - d.toDays();
    int mo = 1;
    while (mo < 12) {
        int dim = d.daysInMonth(mo, y);
        if (rem < dim) break;
        rem -= dim;
        ++mo;
    }
               return Date(static_cast<int>(rem) + 1, mo, y);
}
int Date::dayOfYear() const
{
    int doy = m_day;
    for (int mo = 1; mo < m_month; ++mo)
        doy += daysInMonth(mo, m_year);
    return doy;
}

short Date::weekNumber() const
{
    int doy = dayOfYear();
    // day of week for Jan 1 (0=Mon..6=Sun)
    Date jan1(1, 1, m_year);
    long d1 = jan1.toDays();
    int dow1 = static_cast<int>((d1 + 6) % 7);  // Mon=0

    int week = (doy + dow1 - 1) / 7 + 1;
    return static_cast<short>(std::max(1, std::min(week, 53)));
}
int Date::duration(const Date& other) const
{
    return static_cast<int>(other.toDays() - toDays());
}
int Date::daysTillYourBirthday(const Date& birthdayDate) const
{
   
    Date next(birthdayDate.day(), birthdayDate.month(), m_year);
    if (!next.isValid()) {
       
        next = Date(birthdayDate.day(), birthdayDate.month(), m_year);
    }
    if (next <= *this) {
        next = Date(birthdayDate.day(), birthdayDate.month(), m_year + 1);
        if (!next.isValid())
            next = Date(birthdayDate.day(), birthdayDate.month(), m_year + 1);
    }
    return duration(next);
}


std::string Date::toString() const
{
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << m_day   << "."
        << std::setw(2) << m_month << "."
        << std::setw(4) << m_year;
    return oss.str();
}
Date Date::fromString(const std::string& s)
{
   //"DD.MM.YYYY"
    if (s.size() < 10 || s[2] != '.' || s[5] != '.')
        throw std::invalid_argument("Bad date format, expected DD.MM.YYYY: " + s);
    int d = std::stoi(s.substr(0, 2));
    int m = std::stoi(s.substr(3, 2));
    int y = std::stoi(s.substr(6));
    return Date(d, m, y);
}


Date Date::today()
{
    std::time_t t = std::time(nullptr);
    std::tm* lt   = std::localtime(&t);
    return Date(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

















bool Date::operator==(const Date& o) const {
    return m_day == o.m_day && m_month == o.m_month && m_year == o.m_year;
}
bool Date::operator<(const Date& o) const {
    return toDays() < o.toDays();
}
bool Date::operator<=(const Date& o) const {
    return toDays() <= o.toDays();
}
