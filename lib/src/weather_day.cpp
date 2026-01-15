#include "weather_day.hpp"

using namespace std;

void WeatherDay::choicePhenomen() {
    Phenomen p = parts_of_day.getPhenomen();
    if (precipitation > 0 && p < Phenomen::Rainy) phenomen = Phenomen::Rainy;
    else phenomen = p;
}

WeatherDay::WeatherDay(Date dt, double newprecipitation, PartsOfDay newparts, int phenomen) {
    date = dt;
    setPrecipitation(newprecipitation);
    parts_of_day = newparts;
    setPhenomen(phenomen);
}

WeatherDay::WeatherDay():
    date(),
    parts_of_day(),
    phenomen(Phenomen::Sunny),
    precipitation(0.0) {}


WeatherDay::WeatherDay(Date dt, double newprecipitation, PartsOfDay parts) {
    date = dt;
    setPrecipitation(newprecipitation);
    parts_of_day = parts;
    choicePhenomen();
}

void WeatherDay::setPrecipitation(double new_precipitation) {
    if(new_precipitation < 0) throw invalid_argument("INVALID_ARGUMENT(precipitation)");
    precipitation = new_precipitation;
}

void WeatherDay::setPhenomen(int index) {
    phenomen = static_cast<Phenomen>(index);
}

void WeatherDay::setPhenomen(Phenomen new_phenomen) {
    phenomen = new_phenomen;
}


PartsOfDay WeatherDay::getPartsOfDay() const {
    return parts_of_day;
}

double WeatherDay::getPrecipitation() const{
    return precipitation;
}

Phenomen WeatherDay::getPhenomen() const{
    return phenomen;
}

Date WeatherDay::getDate() const {
    return date;
}

string WeatherDay::getPhenomenForPrint() const{
    switch (phenomen) {
        case Phenomen::Sunny:  return "SUNNY";
        case Phenomen::Cloudy: return "CLOUDY";
        case Phenomen::Rainy:  return "RAINY";
        case Phenomen::Snowy:  return "SNOWY";
        default: return "UNKNOW";
    }
}

bool WeatherDay::check() const {
    if((parts_of_day.getMorning().getTemperature() > 60 || parts_of_day.getMorning().getTemperature() < -100)
        ||(parts_of_day.getDay().getTemperature() > 60 || parts_of_day.getDay().getTemperature() < -100)
        ||(parts_of_day.getEvening().getTemperature() > 60 || parts_of_day.getEvening().getTemperature() < -100))
        return false;
    if((phenomen == Phenomen::Sunny || phenomen == Phenomen::Cloudy) && precipitation == 0)
        return false;
    if(phenomen == Phenomen::Snowy && (parts_of_day.getMorning().getTemperature() > 0
        || parts_of_day.getDay().getTemperature() > 0
        || parts_of_day.getEvening().getTemperature() > 0))
        return false;
    return true;
}

int WeatherDay::averageTempOfDay() const{
    return (parts_of_day.getMorning().getTemperature()
          + parts_of_day.getDay().getTemperature()
          + parts_of_day.getEvening().getTemperature())/3;
}

WeatherDay& WeatherDay::operator+=(const WeatherDay& other) {
    if(this->date == other.date) {
        this->parts_of_day.getMorning().setTemperature(
            (parts_of_day.getMorning().getTemperature() 
            + other.parts_of_day.getMorning().getTemperature())/2);
        this->parts_of_day.getDay().setTemperature(
            (parts_of_day.getDay().getTemperature() 
            + other.parts_of_day.getDay().getTemperature())/2);
        this->parts_of_day.getEvening().setTemperature(
            (parts_of_day.getEvening().getTemperature() 
            + other.parts_of_day.getEvening().getTemperature())/2);
        this->setPrecipitation((getPrecipitation() + other.getPrecipitation()) / 2);
        if(static_cast<int>(this->getPhenomen()) < static_cast<int>(other.phenomen)) this->setPhenomen(other.getPhenomen());
        return *this;
        }
    else throw invalid_argument("INVALID DATE\n");
}

ostream& operator<<(std::ostream& os, const WeatherDay& obj) {
    os << "DATE:" << obj.getDate().getDay() << "." 
                  << obj.getDate().getMonth() << "."
                  << obj.getDate().getYear() << "\n" 
        << "Phenomen of weather: " << obj.getPhenomenForPrint() << "\n"
        << "Temperature of morning: " << obj.getPartsOfDay().getMorning().getTemperature() << "\n"
        << "Temperature of day: " << obj.getPartsOfDay().getDay().getTemperature() << "\n"
        << "Temperature of evening: " << obj.getPartsOfDay().getEvening().getTemperature() << "\n"
        << "Precipitation: " << obj.getPrecipitation() << endl;
    return os;
}

std::istream& operator>>(std::istream& is, WeatherDay& obj) {
    Date d; 
    double prec;
    int t1, t2, t3;
    if (is >> d >> prec >> t1 >> t2 >> t3) {
        obj.date = d;
        obj.setPrecipitation(prec);
        
        Weather w1, w2, w3;
        w1.setTemperature(t1);
        w2.setTemperature(t2);
        w3.setTemperature(t3);
        
        PartsOfDay parts;
        parts.setMorning(w1);
        parts.setDay(w2);
        parts.setEvening(w3);
        obj.parts_of_day = parts;
        
        obj.choicePhenomen();
    }
    return is;
}
