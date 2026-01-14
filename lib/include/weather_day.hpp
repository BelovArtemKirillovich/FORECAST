#ifndef WEATHERDAY_HPP
#define WEATHERDAY_HPP

#include <iostream>
#include <iomanip>
#include <string>

#include "date.hpp"
#include "parts_of_day.hpp"
#include "weather.hpp"


class WeatherDay {
private:
    Date date;
    PartsOfDay parts_of_day;
    Phenomen phenomen;
    double precipitation;
    void choicePhenomen();
public:
    WeatherDay();
    WeatherDay(Date dt, double newprecipitation, PartsOfDay parts);
    WeatherDay(Date dt, double newprecipitation, PartsOfDay newparts, int phenomen);
    
    void setPrecipitation(double new_precipitation);
    void setPhenomen(Phenomen newphenomen);
    void setPhenomen(int index);
 
    PartsOfDay getPartsOfDay() const;
    double getPrecipitation() const;
    Phenomen getPhenomen() const;
    Date getDate() const;
    string getPhenomenForPrint() const;
    
    bool check() const;
    int averageTempOfDay() const;

    WeatherDay& operator+=(const WeatherDay& other);
    friend ostream& operator<<(std::ostream& os, const WeatherDay& obj);
};

#endif
