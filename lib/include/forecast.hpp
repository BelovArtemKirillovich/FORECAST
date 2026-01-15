#ifndef FORECAST_HPP
#define FORECAST_HPP

#include "weather_day.hpp"
#include <algorithm>

class Forecast {
private:
    WeatherDay *data;
    size_t count = 0, capacity = 0;
    void resize(size_t new_capacity);
public:
    Forecast();
    Forecast(WeatherDay* new_data, size_t new_capacity);
    Forecast(size_t initial_capacity);

    ~Forecast();

    Forecast(const Forecast& other);
    Forecast(Forecast&& other);

    void deleteByIndex(size_t index);
    void deleteAllErrors();

    WeatherDay findColdestDay(Date from, Date to);
    WeatherDay findNextSunnyDay(const Date& today);
    Forecast giveAllDaysOfMonth(size_t month);

    void sortDaysByData();
    void mergeDaysByData();
    
    Forecast& operator+=(const WeatherDay& newday);
    WeatherDay& operator[](size_t index);
    Forecast& operator=(const Forecast& other);
    Forecast& operator=(Forecast&& other) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Forecast& obj);
};

#endif