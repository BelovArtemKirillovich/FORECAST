#include "weather.hpp"

using namespace std;

Weather::Weather() : temperature{0} {}

void Weather::setTemperature(int newtemperature) {
    if(newtemperature < -273) throw invalid_argument("INVALID_ARGUMENT(temperature)");
    temperature = newtemperature;
}

int Weather::getTemperature() const{
    return temperature;
}

Phenomen Weather::getPhenomen() const {
    if (temperature < 0) return Phenomen::Snowy;
    if (temperature > 25) return Phenomen::Sunny;
    return Phenomen::Cloudy;
}