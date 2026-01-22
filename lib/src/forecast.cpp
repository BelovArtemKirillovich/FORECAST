#include "forecast.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

using namespace std;

void Forecast::resize(size_t new_capacity) {
    WeatherDay* newdata = new WeatherDay[new_capacity];
    capacity = new_capacity;
    copy_n(data, count, newdata);
    delete [] data;
    data = newdata;
}

Forecast::Forecast(): count(0), capacity(1) {
    data = new WeatherDay[1];
}

Forecast::Forecast(WeatherDay* new_data, size_t new_capacity): count(new_capacity), capacity(new_capacity) {
    if(!new_data) throw invalid_argument("INVALID DATA\n");
    data = new_data;
}

Forecast::Forecast(size_t initial_capacity): count(0), capacity(initial_capacity) {
    if (initial_capacity == 0) throw "INVALID CAPACITY\n";
    data = new WeatherDay[initial_capacity];
}

Forecast::~Forecast() {
    delete[] data;
}

Forecast::Forecast(const Forecast& other): count(other.count), capacity(other.capacity) {
    if (other.data == nullptr) throw "DATA IS EMPTY\n";
    if (capacity == 0) throw "INVALID CAPACITY\n";
    data = new WeatherDay[capacity];
    copy(other.data, other.data + count, data);
}

Forecast::Forecast(Forecast&& other): data(other.data), count(other.count), capacity(other.capacity) {
    other.data = nullptr;
    other.count = 0;
    other.capacity = 0;
}

void Forecast::deleteByIndex(size_t index) {
    if (index >= count) throw invalid_argument("INVALID INDEX\n");
    for (size_t i = index; i != count - 1; i++) {
        data[i] = data[i + 1];
    }
    --count;
    if(count < capacity / 3) resize(capacity / 2);
}

void Forecast::deleteAllErrors() {
    if (count == 0) return;
    size_t deleted_count = 0;
    auto new_end = remove_if(
        data, 
        data + count, 
        [&deleted_count](WeatherDay& day) { 
            bool result = !day.check();
            if (result) deleted_count++;
            return result; 
        }
    );
    count = distance(data, new_end);
}

WeatherDay Forecast::findColdestDay(Date from, Date to) {
    if(count == 0) throw invalid_argument("DATA IS EMPTY\n");
    auto filter_view = std::ranges::filter_view(
        std::span(data, count),
        [from, to](const WeatherDay& day) 
        { return day.getDate() > from && day.getDate() < to;}
    );
    auto coldest_day = min_element(
        filter_view.begin(), 
        filter_view.end(), 
        [](const WeatherDay& a, const WeatherDay& b) 
        { return a.averageTempOfDay() < b.averageTempOfDay();}
    );
    return *coldest_day;
}

WeatherDay Forecast::findNextSunnyDay(const Date& today) {
    if (count == 0) throw std::invalid_argument("DATA IS EMPTY");
    auto filter_view = std::ranges::filter_view(
        std::span(data, count),
        [today](const WeatherDay& day) 
        { return day.getPhenomen() == Phenomen::Sunny && day.getDate() > today;}
    );
    if(filter_view.empty()) throw std::runtime_error("No sunny day found after the given date");
    auto result = min_element(
        filter_view.begin(), 
        filter_view.end(), 
        [](const WeatherDay& a, const WeatherDay& b)
        { return a.getDate() < b.getDate(); });
    return *result;
}

Forecast Forecast::giveAllDaysOfMonth(size_t month) {
    if (count == 0) throw invalid_argument("DATA IS EMPTY\n");
    if (month > 12 || month == 0) throw invalid_argument("INVALID MONTH\n");
    auto view = std::ranges::filter_view(
        std::span(data, count),
        [month](const WeatherDay& day) 
        { return day.getDate().getMonth() == month;}
    );
    if(view.empty()) throw std::runtime_error("There is no weather forecast for this month.\n");
    Forecast result;
    for(auto& day : view) result += day;
    result.sortDaysByData();
    return result;
}

void Forecast::sortDaysByData() {
    sort(
        data,
        data+count,
        [](const WeatherDay& a, const WeatherDay& b)
        { return a.getDate() < b.getDate(); }
    ); 
}

void Forecast::mergeDaysByData() {
    for(size_t i = count - 1; i != 0; i--) {
        WeatherDay& day = data[i];
        size_t j = i - 1;
        while(true) {
            if(day.getDate() == data[j].getDate()) {
                day += data[j];
                deleteByIndex(j);
            }
            if(j == 0) break;
            j--;
        }
    }
}

Forecast& Forecast::operator+=(const WeatherDay& new_day) {
    if(count == capacity) resize(capacity * 2);
    cout << "BHKNTGN\n";
    data[count++] = new_day;
    return *this;
}

WeatherDay& Forecast::operator[](size_t index) {
    if (index >= count) throw out_of_range("INVALID INDEX");
        return data[index];
}

Forecast& Forecast::operator=(const Forecast& other) {
    if (this == &other) return *this;
    if (other.data != nullptr) {
        WeatherDay* new_data = new WeatherDay[other.capacity];
        copy(other.data, other.data + other.count, new_data);
        delete [] data;
        data = new_data;
        count = other.count;
        capacity = other.capacity;
    } else {
        data = nullptr;
        capacity = 0;
        count = 0;
    }
    return *this;
}

Forecast& Forecast::operator=(Forecast&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;
    data = other.data;
    count = other.count;
    capacity = other.capacity;
    other.data = nullptr;
    other.capacity = 0;
    other.count = 0;
    return *this;
}


ostream& operator<<(std::ostream& os, const Forecast& obj) {
    os << "===========================" << endl;
    for(size_t index = 0; index != obj.count; index++) {
        os << index + 1 << "." << obj.data[index];
        os << "===========================" << endl;
    }
    return os;
}