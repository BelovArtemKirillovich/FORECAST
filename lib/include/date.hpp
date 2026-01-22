/**
 * @file Date.hpp
 * @brief Определение класса Date для представления календарной даты.
 *
 * Класс Date хранит день, месяц, год как целые числа без проверки корректности
 * (например, 32.13.2026 допустимо на уровне хранения). Предоставляет базовые
 * операции сравнения и ввода/вывода.
 */

#ifndef DATE_HPP
#define DATE_HPP

#include <cstdint>
#include <istream>

/**
 * @class Date
 * @brief Представляет календарную дату стандарта ISO (день, месяц, год).
 *
 * Хранит компоненты даты как отдельные целочисленные поля. Не выполняет
 * автоматической валидации (например, не проверяет, существует ли 30 февраля).
 * Поддерживает сравнение дат и потоковый ввод из std::istream.
 */
class Date {
private:
    uint32_t day;   ///< День месяца (1–31)
    uint32_t month; ///< Месяц года (1–12)
    int32_t year;   ///< Год (может быть отрицательным)

public:
    /**
     * @brief Конструктор по умолчанию.
     *
     * Инициализирует дату как 1.1.1970.
     */
    Date();

    /**
     * @brief Конструктор с параметрами.
     *
     * @param new_day   День месяца
     * @param new_month Месяц года
     * @param new_year  Год
     *
     * @note Не выполняет проверку корректности даты (например, 32.13.2026 допустимо).
     */
    Date(uint32_t new_day, uint32_t new_month, int32_t new_year);

    /**
     * @brief Устанавливает новый день.
     * @param new_day Новое значение дня месяца
     */
    void setDay(uint32_t new_day);

    /**
     * @brief Устанавливает новый месяц.
     * @param new_month Новое значение месяца
     */
    void setMonth(uint32_t new_month);

    /**
     * @brief Устанавливает новый год.
     * @param new_year Новое значение года
     */
    void setYear(int32_t new_year);

    /**
     * @brief Возвращает день месяца.
     * @return Значение дня (uint32_t)
     */
    uint32_t getDay() const;

    /**
     * @brief Возвращает месяц года.
     * @return Значение месяца (uint32_t)
     */
    uint32_t getMonth() const;

    /**
     * @brief Возвращает год.
     * @return Значение года (int32_t)
     */
    int32_t getYear() const;

    /**
     * @brief Выводит дату в формате "DD.MM.YYYY" в стандартный поток вывода.
     *
     * Пример: 22.01.2026
     */
    void print() const;

    /**
     * @brief Оператор "больше чем" для сравнения дат.
     *
     * Сравнение выполняется лексикографически: сначала год, затем месяц, затем день.
     *
     * @param first  Первая дата
     * @param second Вторая дата
     * @return true, если first > second; иначе false
     */
    friend bool operator>(const Date& first, const Date& second);

    /**
     * @brief Оператор "меньше чем" для сравнения дат.
     *
     * Сравнение выполняется лексикографически: сначала год, затем месяц, затем день.
     *
     * @param first  Первая дата
     * @param second Вторая дата
     * @return true, если first < second; иначе false
     */
    friend bool operator<(const Date& first, const Date& second);

    /**
     * @brief Оператор равенства для дат.
     *
     * @param first  Первая дата
     * @param second Вторая дата
     * @return true, если все компоненты (день, месяц, год) равны; иначе false
     */
    friend bool operator==(const Date& first, const Date& second);

    /**
     * @brief Потоковый оператор ввода для Date.
     *
     * Ожидает формат: день.месяц.год (например, 22.01.2026)
     * Читает день, точку, месяц, точку и год.
     *
     * @param is  Входной поток
     * @param obj Объект Date для заполнения
     * @return Ссылка на входной поток после чтения
     *
     * @note Устанавливает флаг failbit, если формат нарушен (например, вместо точки — другой символ).
     */
    friend std::istream& operator>>(std::istream& is, Date& obj);
};

#endif // DATE_HPP