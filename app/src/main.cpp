#include <iostream>
#include <stdexcept>
#include "weather_lib.hpp"
using namespace std;

void menu() {
    cout << "0.Exit" << "\n" 
        << "0. Exit" << "\n" 
        << "1. Add new forecast" << "\n" 
        << "2. Delete forecast by number" << "\n" 
        << "3. Delete all error weathers" << "\n"
        << "4. Find coldest day" << "\n" 
        << "5. Find next sunny day" << "\n" 
        << "6. Merge forecasts with the same date" << "\n"
        << "7. Sort forecasts by date" << "\n"
        << "8. Get sort forecasts by month" << "\n"
        << "9. Import forecasts from file" << "\n";            // TODO
}

int main() {
    Forecast f1 = Forecast();
    while(true) {
        size_t command;
        cin >> command;
        if(command == 0) break;
        switch(command) {
            case 1:
                WeatherDay new_weather_day;
                try {
                cin >> new_weather_day;
                }
                catch (const invalid_argument& er) {
                    std::cerr << er.what() << std::endl;
                }
                f1 += new_weather_day;
                break;
            case 2:
            cout << "Input number of forecast " << endl;
                size_t number;
                cin >> number;
                try {
                    f1.deleteByIndex(--number);
                }
                catch (const invalid_argument& er) {
                    std::cerr << er.what() << std::endl; 
                }
                break;
            case 3:
                f1.deleteAllErrors();
                break;
            case 4:
                Date a, b;
                cout << "Find from:" << endl;
                try {
                    cin >> a;
                }
                catch (const invalid_argument& er) {
                    std::cerr << er.what() << std::endl;
                }
                cout << "Find to:" << endl;
                try {
                    cin >> b;
                }
                catch (const invalid_argument& er) {
                    std::cerr << er.what() << std::endl;
                }
                auto result = f1.findColdestDay(a, b);
                cout << "The coldest day " << "\n" << result;
                break;
            case 5:
                Date a;
                cout << "Date of today:" << endl;
                try {
                    cin >> a;
                }
                catch (const invalid_argument& er) {
                    std::cerr << er.what() << std::endl;
                }
                cout << "The next sunny day\n" << f1.findNextSunnyDay(a);
                break;
            case 6:
                f1.mergeDaysByData();
                break;
            case 7:
                f1.sortDaysByData();
                break;
            case 8:
                size_t month;
                cin >> month;
                try {
                    cout << f1.giveAllDaysOfMonth(month);
                }
                catch (const invalid_argument& er) {
                    std::cerr << er.what() << std::endl;
                }
                break;
            case 9:
                break;
        }
        cout << f1;
    }
    f1.~Forecast();
    return 0;
}