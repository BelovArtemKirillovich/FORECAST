#ifndef PARTSOFDAY_HPP
#define PARTSOFDAY_HPP

#include "weather.hpp"


class PartsOfDay {
private:
    Weather morning,
            day,
            evening;
public:
    PartsOfDay();

    void setMorning(Weather new_weather);
    void setDay(Weather new_weather);
    void setEvening(Weather new_weather);

    void setMorning(int new_temperature);
    void setDay(int new_temperature);
    void setEvening(int new_temperature);

    Weather getMorning() const;
    Weather getDay() const;
    Weather getEvening() const;

    
    Phenomen getPhenomen() const;
};

#endif