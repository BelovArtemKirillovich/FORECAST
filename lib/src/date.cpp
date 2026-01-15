#include <iostream>
#include <cstdint>
#include <iomanip>
#include "date.hpp"

using namespace std;

Date::Date(): day{1}, month{1}, year{1970} {} // Must be init by today

Date::Date(uint8_t new_day, uint8_t new_month, int16_t new_year) {
    setDay(new_day);
    setMonth(new_month);
    setYear(new_year);
}

void Date::setDay(uint8_t new_day) {
    if(day > 31) throw invalid_argument("INVALID_ARGUMENT(day)");
    day = new_day;
}

void Date::setMonth(uint8_t new_month) {
    if(month > 12) throw invalid_argument("INVALID_ARGUMENT(month)");
    month = new_month;
}

void Date::setYear(int16_t new_year) {
    if (year > 9999 || year < -999) throw invalid_argument("INVALID_ARGUMENT(year)");
    year = new_year;
}

uint8_t Date::getDay() const{
    return day;
}

uint8_t Date::getMonth() const{
    return month;
}

int16_t Date::getYear() const{
    return year;
}

void Date::print() const{
    cout << setw(2) << day << "."
        << setw(2) << month << "."
        << setw(4) << year << endl; // Can ISO Date standard
}

bool operator>(const Date& first, const Date& second) {
    if (first.year > second.year) return true;
    if (first.month > second.month) return true;
    return first.day > second.day;
}

bool operator<(const Date& first, const Date& second) {
    if (first.year < second.year) return true;
    if (first.month < second.month) return true;
    return first.day < second.day;
}

bool operator==(const Date& first, const Date& second) {
    if(first.year == second.year &&
        first.month == second.month &&
       first.day == second.day) return true;
    else return false;
}

istream& operator>>(std::istream& is, Date& obj) {
    int d, m, y;
    char dot1, dot2;
    if (is >> d >> dot1 >> m >> dot2 >> y) {
        if (dot1 == '.' && dot2 == '.') {
            try {
                obj.setDay(d);
                obj.setMonth(m);
                obj.setYear(y);
            } catch(...) {
                is.setstate(std::ios::failbit);
            }
        } else {
            is.setstate(std::ios::failbit);
        }
    }
    return is;
}
