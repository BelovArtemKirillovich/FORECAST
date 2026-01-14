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

    void setMorning(Weather newweather);
    void setDay(Weather newweather);
    void setEvening(Weather newweather);

    Weather getMorning() const;
    Weather getDay() const;
    Weather getEvening() const;

    
    Phenomen getPhenomen() const;
};

#endif