#include "weather.hpp"

using namespace std;

Weather::Weather() : temperature{0} {}

void Weather::setTemperature(int new_temperature) {
    if(new_temperature < -273) throw invalid_argument("INVALID_ARGUMENT(temperature)");
    temperature = new_temperature;
}

int Weather::getTemperature() const{
    return temperature;
}

Phenomen Weather::getPhenomen() const {
    if (temperature < 0) return Phenomen::Snowy;
    if (temperature > 25) return Phenomen::Sunny;
    return Phenomen::Cloudy;
}