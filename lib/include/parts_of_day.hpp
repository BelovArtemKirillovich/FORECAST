/**
 * @file parts_of_day.hpp
 * @brief Определение класса PartsOfDay — представление показателей средней температуры по частям дня (утро, день, вечер).
 *
 * Класс хранит три объекта Weather, соответствующих утру, дню и вечеру,
 * и предоставляет методы для их установки, получения и определения
 * доминирующего погодного явления за день.
 */

#ifndef PARTSOFDAY_HPP
#define PARTSOFDAY_HPP

#include "weather.hpp"
#include <algorithm>

/**
 * @class PartsOfDay
 * @brief Представляет погодные условия в три основных периода суток: утро, день, вечер.
 *
 * Содержит три независимых объекта Weather. Позволяет задавать погоду как целиком (через Weather),
 * так и только температуру (остальные параметры Weather устанавливаются автоматически).
 *
 * Доминирующее погодное явление определяется как максимальное значение из трёх
 * по порядку в перечислении Phenomen (Sunny=1, Cloudy=2, Rainy=3, Snowy=4).
 */
class PartsOfDay {
private:
    Weather morning;  ///< Погода утром
    Weather day;      ///< Погода днём
    Weather evening;  ///< Погода вечером

public:
    /**
     * @brief Конструктор по умолчанию.
     *
     * Инициализирует утро, день и вечер значениями по умолчанию (температура = 0°C, явление = Sunny).
     */
    PartsOfDay();

    /**
     * @brief Устанавливает погоду утром.
     * @param new_weather Новый объект Weather для утра.
     */
    void setMorning(Weather new_weather);

    /**
     * @brief Устанавливает погоду днём.
     * @param new_weather Новый объект Weather для дня.
     */
    void setDay(Weather new_weather);

    /**
     * @brief Устанавливает погоду вечером.
     * @param new_weather Новый объект Weather для вечера.
     */
    void setEvening(Weather new_weather);

    /**
     * @brief Устанавливает температуру утром.
     * @param new_temperature Новая температура утром (°C).
     * @throws std::invalid_argument если температура < −273°C.
     */
    void setMorning(int new_temperature);

    /**
     * @brief Устанавливает температуру днём.
     * @param new_temperature Новая температура днём (°C).
     * @throws std::invalid_argument если температура < −273°C.
     */
    void setDay(int new_temperature);

    /**
     * @brief Устанавливает температуру вечером.
     * @param new_temperature Новая температура вечером (°C).
     * @throws std::invalid_argument если температура < −273°C.
     *
     * @note В текущей реализации есть ошибка: температура устанавливается для утра, а не для вечера.
     */
    void setEvening(int new_temperature);

    /**
     * @brief Возвращает погоду утром.
     * @return Копия объекта Weather для утра.
     */
    Weather getMorning() const;

    /**
     * @brief Возвращает погоду днём.
     * @return Копия объекта Weather для дня.
     */
    Weather getDay() const;

    /**
     * @brief Возвращает погоду вечером.
     * @return Копия объекта Weather для вечера.
     */
    Weather getEvening() const;

    /**
     * @brief Определяет доминирующее погодное явление за день.
     *
     * Возвращает явление с наибольшим значением в перечислении Phenomen:
     *  Sunny = 1
     *  Cloudy = 2
     *  Rainy = 3
     *  Snowy = 4
     *
     * Пример: если утром Sunny, днём Rainy, вечером Snowy → вернёт Snowy.
     *
     * @return Элемент перечисления Phenomen.
     */
    Phenomen getPhenomen() const;
};

#endif // PARTSOFDAY_HPP