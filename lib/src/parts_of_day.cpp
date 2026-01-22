#include "parts_of_day.hpp"

PartsOfDay::PartsOfDay() :
    morning{},
    day {},
    evening{}
{};

void PartsOfDay::setMorning(Weather newweather) {
    morning = newweather;
}

void PartsOfDay::setDay(Weather newweather) {
    day = newweather;
}

void PartsOfDay::setEvening(Weather newweather) {
    evening = newweather;
}

void PartsOfDay::setMorning(int new_temperature) {
    this->morning.setTemperature(new_temperature);
}

void PartsOfDay::setDay(int new_temperature) {
    this->day.setTemperature(new_temperature);
}
void PartsOfDay::setEvening(int new_temperature) {
    this->evening.setTemperature(new_temperature);
}

Weather PartsOfDay::getMorning() const {
    return morning;
}

Weather PartsOfDay::getDay() const {
    return day;
}

Weather PartsOfDay::getEvening() const {
    return evening;
}

Phenomen PartsOfDay::getPhenomen() const {
    return std::max(
        std::max(morning.getPhenomen(), day.getPhenomen()),
        evening.getPhenomen()
    );
}