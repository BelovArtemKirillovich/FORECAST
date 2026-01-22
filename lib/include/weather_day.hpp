/**
 * @file weather_day.hpp
 * @brief Определение класса WeatherDay — представление погоды за один календарный день.
 *
 * Класс объединяет дату, осадки, погодные условия утром/днём/вечером и общее
 * погодное явление дня. Поддерживает ввод/вывод, проверку корректности данных,
 * вычисление средней температуры и добавление новых прогнозов через оператор +=.
 */

#ifndef WEATHERDAY_HPP
#define WEATHERDAY_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

#include "date.hpp"
#include "parts_of_day.hpp"
#include "weather.hpp"

/**
 * @class WeatherDay
 * @brief Представляет полную информацию о погоде за одни сутки.
 *
 * Содержит:
 * - дату (Date),
 * - осадки (double, ≥ 0),
 * - погоду по частям дня (PartsOfDay),
 * - общее погодное явление (Phenomen), определяемое автоматически или вручную.
 *
 * Автоматическое определение явления (`choicePhenomen()`) учитывает:
 * - температуру (через PartsOfDay → Weather → Phenomen),
 * - наличие осадков: если precipitation > 0 и явление не сильнее Rainy — устанавливается Rainy.
 *
 * @note Класс поддерживает потоковый ввод/вывод, проверку корректности (`check()`)
 *       и усреднение двух прогнозов на одну дату (`operator+=`).
 */
class WeatherDay {
private:
    Date date;                  ///< Дата прогноза
    PartsOfDay parts_of_day;    ///< Части дня: утро, день и вечер
    Phenomen phenomen;          ///< Общее погодное явление дня
    double precipitation;       ///< Количество осадков (в условных единицах, ≥ 0)

    /**
     * @brief Автоматически определяет погодное явление на основе температуры и осадков.
     *
     * Логика:
     * - Получает базовое явление из `parts_of_day.getPhenomen()` (на основе температур).
     * - Если `precipitation > 0` и текущее явление слабее Rainy (Sunny/Cloudy) → устанавливает Rainy.
     * - Иначе сохраняет базовое явление.
     *
     * Вызывается при конструировании и чтении из потока.
     */
    void choicePhenomen();

public:
    /**
     * @brief Конструктор по умолчанию.
     *
     * Инициализирует:
     * - дату как 1.1.1970,
     * - осадки = 0.0,
     * - части дня значениями по умолчанию,
     * - явление = Sunny.
     */
    WeatherDay();

    /**
     * @brief Конструктор с автоматическим определением явления.
     *
     * @param dt             Дата
     * @param newprecipitation Количество осадков (≥ 0)
     * @param parts          Погода по частям дня
     *
     * @note Явление определяется автоматически через `choicePhenomen()`.
     * @throws std::invalid_argument если осадки < 0.
     */
    WeatherDay(Date dt, double newprecipitation, PartsOfDay parts);

    /**
     * @brief Конструктор с явным указанием явления через целое число.
     *
     * @param dt             Дата
     * @param newprecipitation Количество осадков (≥ 0)
     * @param newparts       Погода по частям дня
     * @param phenomen       Индекс явления (1=Sunny, 2=Cloudy, 3=Rainy, 4=Snowy)
     *
     * @note Не вызывает `choicePhenomen()` — явление задаётся вручную.
     * @throws std::invalid_argument если осадки < 0.
     */
    WeatherDay(Date dt, double newprecipitation, PartsOfDay newparts, int phenomen);

    /**
     * @brief Устанавливает количество осадков.
     * @param new_precipitation Новое значение осадков (должно быть ≥ 0)
     * @throws std::invalid_argument если значение < 0.
     */
    void setPrecipitation(double new_precipitation);

    /**
     * @brief Устанавливает погодное явление.
     * @param newphenomen Новое значение из перечисления Phenomen.
     */
    void setPhenomen(Phenomen newphenomen);

    /**
     * @brief Устанавливает погодное явление по индексу.
     * @param index Целое число, соответствующее значению Phenomen (1–4).
     * @note Не выполняет проверку допустимости индекса.
     */
    void setPhenomen(int index);

    /**
     * @brief Возвращает погоду по частям дня.
     * @return Объект PartsOfDay (копия).
     */
    PartsOfDay getPartsOfDay() const;

    /**
     * @brief Возвращает количество осадков.
     * @return Осадки (double, ≥ 0).
     */
    double getPrecipitation() const;

    /**
     * @brief Возвращает погодное явление дня.
     * @return Элемент перечисления Phenomen.
     */
    Phenomen getPhenomen() const;

    /**
     * @brief Возвращает дату прогноза.
     * @return Объект Date.
     */
    Date getDate() const;

    /**
     * @brief Возвращает строковое представление явления для вывода.
     * @return Одна из строк: "SUNNY", "CLOUDY", "RAINY", "SNOWY", "UNKNOW".
     */
    std::string getPhenomenForPrint() const;

    /**
     * @brief Проверяет корректность данных прогноза.
     *
     * Правила валидации:
     * - Температура в каждой части дня ∈ [−100, 60]°C.
     * - Для Sunny/Cloudy: осадки должны быть 0.
     * - Для Snowy: все температуры ≤ 0 и осадки ≤ 1500.
     *
     * @return true, если данные корректны; иначе false.
     */
    bool check() const;

    /**
     * @brief Вычисляет среднюю температуру дня.
     *
     * Среднее арифметическое температур утра, дня и вечера (целочисленное деление).
     *
     * @return Средняя температура (int).
     */
    int averageTempOfDay() const;

    /**
     * @brief Усредняет два прогноза на одну и ту же дату.
     *
     * Обновляет текущий объект:
     * - Температуры усредняются (целочисленно).
     * - Осадки усредняются.
     * - Явление выбирается как более «сильное» (с бóльшим числовым значением в enum).
     *
     * @param other Другой прогноз на ту же дату.
     * @return Ссылка на *this.
     * @throws std::invalid_argument если даты не совпадают.
     */
    WeatherDay& operator+=(const WeatherDay& other);

    /**
     * @brief Потоковый оператор вывода.
     *
     * Формат:
     * @code
     * DATE:день.месяц.год
     * Phenomen of weather: SUNNY
     * Temperature of morning: 5
     * Temperature of day: 7
     * Temperature of evening: 3
     * Precipitation: 0
     * @endcode
     *
     * @param os  Выходной поток
     * @param obj Объект WeatherDay
     * @return Ссылка на выходной поток.
     */
    friend std::ostream& operator<<(std::ostream& os, const WeatherDay& obj);

    /**
     * @brief Потоковый оператор ввода.
     *
     * Ожидает формат: день.месяц.год <осадки> <утро> <день> <вечер>
     * Пример: 22.01.2026 0.0 5 7 3
     *
     * После чтения автоматически вызывает `choicePhenomen()`.
     *
     * @param is  Входной поток
     * @param obj Объект WeatherDay для заполнения
     * @return Ссылка на входной поток.
     */
    friend std::istream& operator>>(std::istream& is, WeatherDay& obj);
};

#endif // WEATHERDAY_HPP
