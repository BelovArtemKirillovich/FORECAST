/**
 * @file forecast.hpp
 * @brief Определение класса Forecast — динамический контейнер для хранения прогнозов погоды.
 *
 * Класс реализует управляемый массив объектов WeatherDay с автоматическим расширением,
 * поддержкой копирования, перемещения, поиска, фильтрации и модификации данных.
 * Использует стратегию удвоения ёмкости при переполнении и уменьшения при сильном опустошении.
 */

#ifndef FORECAST_HPP
#define FORECAST_HPP

#include "weather_day.hpp"
#include <algorithm>
#include <stdexcept>
#include <cstddef>

/**
 * @class Forecast
 * @brief Контейнер для хранения и управления прогнозами погоды по дням.
 *
 * Хранит массив объектов WeatherDay с динамическим управлением памятью.
 * Поддерживает:
 * - добавление, удаление, доступ по индексу,
 * - поиск по условиям (самый холодный день, ближайший солнечный и т.д.),
 * - фильтрацию (например, все дни месяца),
 * - сортировку и объединение записей с одинаковой датой.
 *
 * @note Внутренний массив может быть больше количества элементов (`capacity >= count`).
 * @warning Не все методы проверяют корректность входных данных — см. описание каждого метода.
 */
class Forecast {
private:
    WeatherDay* data;     ///< Указатель на динамический массив прогнозов
    size_t count;         ///< Текущее количество элементов
    size_t capacity;      ///< Выделенная ёмкость массива

    /**
     * @brief Изменяет ёмкость внутреннего массива.
     *
     * Выделяет новый массив указанного размера, копирует существующие данные,
     * освобождает старый массив.
     *
     * @param new_capacity Новая ёмкость (должна быть ≥ count)
     * @note Не изменяет значение `count`.
     */
    void resize(size_t new_capacity);

public:
    /**
     * @brief Конструктор по умолчанию.
     *
     * Создаёт пустой контейнер с начальной ёмкостью 1.
     */
    Forecast();

    /**
     * @brief Конструктор перемещения владения внешним массивом.
     *
     * Принимает готовый массив и передаёт ему владение.
     *
     * @param new_data Указатель на массив WeatherDay (не nullptr)
     * @param new_capacity Размер массива
     * @throws std::invalid_argument если new_data == nullptr
     * @warning После вызова ответственность за освобождение памяти переходит к объекту Forecast.
     */
    Forecast(WeatherDay* new_data, size_t new_capacity);

    /**
     * @brief Конструктор с заданной начальной ёмкостью.
     *
     * @param initial_capacity Начальная ёмкость (должна быть > 0)
     * @throws const char* ("INVALID CAPACITY\n") если initial_capacity == 0
     */
    Forecast(size_t initial_capacity);

    /**
     * @brief Деструктор.
     *
     * Освобождает выделенную память.
     */
    ~Forecast();

    /**
     * @brief Конструктор копирования.
     *
     * Создаёт глубокую копию другого Forecast.
     *
     * @param other Исходный объект
     * @throws const char* если other.data == nullptr или capacity == 0
     */
    Forecast(const Forecast& other);

    /**
     * @brief Конструктор перемещения.
     *
     * Передаёт владение ресурсами от `other` к текущему объекту.
     * Оставляет `other` в пустом, но валидном состоянии.
     *
     * @param other Источник (rvalue)
     */
    Forecast(Forecast&& other);

    /**
     * @brief Удаляет прогноз по индексу.
     *
     * Сдвигает последующие элементы влево.
     * При сильном опустошении (count < capacity/3) уменьшает ёмкость в 2 раза.
     *
     * @param index Индекс удаляемого элемента (должен быть < count)
     * @throws std::invalid_argument если index >= count
     */
    void deleteByIndex(size_t index);

    /**
     * @brief Удаляет все некорректные прогнозы (для которых check() == false).
     *
     * Сохраняет относительный порядок оставшихся элементов.
     * Не изменяет ёмкость массива.
     */
    void deleteAllErrors();

    /**
     * @brief Находит самый холодный день в заданном диапазоне дат.
     *
     * Диапазон: (from, to) — исключает границы.
     *
     * @param from Начальная дата (не включается)
     * @param to   Конечная дата (не включается)
     * @return Копия самого холодного WeatherDay
     * @throws std::invalid_argument если контейнер пуст
     * @note Использует std::ranges::filter_view и min_element.
     */
    WeatherDay findColdestDay(Date from, Date to);

    /**
     * @brief Находит ближайший солнечный день после заданной даты.
     *
     * @param today Дата, после которой искать
     * @return Копия первого солнечного дня с минимальной датой
     * @throws std::invalid_argument если контейнер пуст
     * @throws std::runtime_error если подходящий день не найден
     * @note Явление должно быть точно Phenomen::Sunny.
     */
    WeatherDay findNextSunnyDay(const Date& today);

    /**
     * @brief Возвращает все прогнозы для указанного месяца.
     *
     * Результат отсортирован по дате.
     *
     * @param month Номер месяца (1–12)
     * @return Новый объект Forecast, содержащий отфильтрованные дни
     * @throws std::invalid_argument если контейнер пуст или month вне [1,12]
     * @throws std::runtime_error если в указанном месяце нет прогнозов
     */
    Forecast giveAllDaysOfMonth(size_t month);

    /**
     * @brief Сортирует прогнозы по возрастанию даты.
     *
     * Использует std::sort с лямбда-функцией сравнения.
     */
    void sortDaysByData();

    /**
     * @brief Объединяет прогнозы с одинаковой датой.
     *
     * Для каждой даты, встречающейся более одного раза,
     * применяет operator+= к последнему вхождению со всеми предыдущими.
     * Удаляет дубликаты через deleteByIndex.
     *
     * @warning Алгоритм имеет O(n²) сложность и может быть неэффективен для больших данных.
     */
    void mergeDaysByData();

    /**
     * @brief Добавляет новый прогноз в конец контейнера.
     *
     * При необходимости увеличивает ёмкость через resize.
     *
     * @param newday Добавляемый прогноз
     * @return Ссылка на *this
     */
    Forecast& operator+=(const WeatherDay& newday);

    /**
     * @brief Доступ к прогнозу по индексу.
     *
     * @param index Индекс (должен быть < count)
     * @return Ссылка на WeatherDay
     * @throws std::out_of_range если index >= count
     */
    WeatherDay& operator[](size_t index);

    /**
     * @brief Оператор копирующего присваивания.
     *
     * Обеспечивает глубокое копирование.
     * Корректно обрабатывает самоприсваивание.
     *
     * @param other Источник
     * @return Ссылка на *this
     */
    Forecast& operator=(const Forecast& other);

    /**
     * @brief Оператор перемещающего присваивания.
     *
     * Передаёт владение ресурсами, оставляет `other` в пустом состоянии.
     * Помечен как noexcept.
     *
     * @param other Источник (rvalue)
     * @return Ссылка на *this
     */
    Forecast& operator=(Forecast&& other) noexcept;

    /**
     * @brief Потоковый оператор вывода.
     *
     * Выводит все прогнозы в формате:
     * ===========================
     * 1.<прогноз>
     * ===========================
     * 2.<прогноз>
     * ...
     *
     * @param os  Выходной поток
     * @param obj Объект Forecast
     * @return Ссылка на выходной поток
     */
    friend std::ostream& operator<<(std::ostream& os, const Forecast& obj);
};

#endif // FORECAST_HPP