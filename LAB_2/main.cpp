#include <iostream>
#include <fstream>
#include "Date.h"
#include "DynamicArray.h"
#include <QFileDialog>


int main(){

  DynamicArray arr;

    std::ifstream file("dates.txt");
    int d, m, y;
    char dot;

    while (file >> d >> dot >> m >> dot >> y) {
        arr.push(new Date(d, m, y));   
    }
    file.close();// кусок чтения файла








Date birthday(19,12,2007);
Date today(10,3,2026);

Date tomorrow = today.NextDay();//1
std::cout << tomorrow.getDay() << "." << tomorrow.getMonth()<< "." << tomorrow.getYear() << '\n';

Date yesteday = today.PreviousDay();
std::cout << yesteday.getDay() << "." << tomorrow.getMonth()<< "." << tomorrow.getYear() << '\n';






std::cout << today.DaysTillYourBithday(birthday);//1







    return 0;
}