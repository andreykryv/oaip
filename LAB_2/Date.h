#ifndef DATE_H
#define DATE_H

class Date {
private:
    int day;
    int month;
    int year;

public:
    Date(int d, int m, int y);
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    Date NextDay() const;
    Date PreviousDay() const;
    bool IsLeap() const;
    short WeekNumber() const;
    int DaysTillYourBithday(const Date& birthdaydate) const;
    int Duration(const Date& date) const;
};

#endif