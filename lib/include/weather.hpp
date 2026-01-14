#ifndef WEATHER_HPP
#define WEATHER_HPP

#include <iostream>


enum class Phenomen {
    Sunny = 1,
    Cloudy,
    Rainy,
    Snowy
};

class Weather {
private:
    int temperature;
public:
    Weather();
    void setTemperature(int new_temperature);

    int getTemperature() const;

    Phenomen getPhenomen() const;
};

#endif