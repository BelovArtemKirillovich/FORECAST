#include "parts_of_day.hpp"

PartsOfDay::PartsOfDay() :
    morning{}, // инициализация по умолчанию всех погод с вызовом конструктора по умолчанию
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