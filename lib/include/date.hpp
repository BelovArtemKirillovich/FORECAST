#ifndef DATE_HPP
#define DATE_HPP

#include <cstdint>

class Date {
private:
    uint8_t day,
         month;
    int16_t year;
public:
    Date();
    Date(uint8_t new_day, uint8_t new_month, int16_t new_year);

    void setDay(uint8_t new_day);
    void setMonth(uint8_t new_month);
    void setYear(int16_t new_year);

    uint8_t getDay() const;
    uint8_t getMonth() const;
    int16_t getYear() const;

    void print() const;

    friend bool operator>(const Date& first, const Date& second);
    friend bool operator<(const Date& first, const Date& second);
    friend bool operator==(const Date& first, const Date& second);
};

#endif