#pragma once
#include <string>


class Date {
public:
    // Constructor
    Date();
    Date(int day, int month, int year);

    // Get
    int day()  const;
    int month() const;
    int year() const;

    Date  nextDay()  const;   
    Date  previousDay() const;   
    bool  isLeap()  const;   
    short weekNumber() const;  

    
    int   daysTillYourBirthday(const Date& birthdayDate) const;

    // количество дней между датами 
    int   duration(const Date& other) const;

    
    std::string toString() const;   // "ДД.ММ.ГГГГ"
    bool isValid()     const;

    static Date fromString(const std::string& s);   // "ДД.ММ.ГГГГ"
    static Date today();                             //  дата Системыыы

    bool operator==(const Date& o) const;
    bool operator< (const Date& o) const;
    bool operator<=(const Date& o) const;

private:
    int m_day;
    int m_month;
    int m_year;

    int  daysInMonth(int month, int year) const;
    int  dayOfYear()                      const;  
    long toDays()                         const;  // дней с 01.01.0001
    static Date fromDays(long days);
};
