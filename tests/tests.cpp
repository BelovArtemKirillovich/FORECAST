#include <gtest/gtest.h>
#include <cmath>
#include <vector>

#include "date.hpp"
#include "weather.hpp"
#include "parts_of_day.hpp"
#include "weather_day.hpp"
#include "forecast.hpp"


void forecast_days_setup(Forecast& f) {
    Weather w1; w1.setTemperature(-10);
    PartsOfDay p1; p1.setMorning(w1); p1.setDay(w1); p1.setEvening(w1);
    f += WeatherDay(Date(1,1,2023), 0.0, p1, static_cast<int>(Phenomen::Sunny));
    Weather w2; w2.setTemperature(-20);
    PartsOfDay p2; p2.setMorning(w2); p2.setDay(w2); p2.setEvening(w2);
    f += WeatherDay(Date(2,1,2023), 0.0, p2, static_cast<int>(Phenomen::Sunny));
    Weather w3; w3.setTemperature(5);
    PartsOfDay p3; p3.setMorning(w3); p3.setDay(w3); p3.setEvening(w3);
    f += WeatherDay(Date(3,1,2023), 0.0, p3, static_cast<int>(Phenomen::Sunny));
}


TEST(DateTest, ConstructorAndGetters) {
    Date d(15, 10, 2023);
    EXPECT_EQ(d.getDay(), 15);
    EXPECT_EQ(d.getMonth(), 10);
    EXPECT_EQ(d.getYear(), 2023);
}

TEST(DateTest, SettersValidation) {
    Date d;
    EXPECT_THROW(d.setDay(32), std::invalid_argument);
    EXPECT_THROW(d.setMonth(13), std::invalid_argument);
    EXPECT_THROW(d.setYear(10000), std::invalid_argument);
    
    EXPECT_NO_THROW(d.setDay(1));
    EXPECT_NO_THROW(d.setMonth(1));
    EXPECT_NO_THROW(d.setYear(2025));
}

TEST(DateTest, ComparisonOperators) {
    Date d1(1, 1, 2023);
    Date d2(2, 1, 2023);
    Date d3(1, 2, 2023);
    Date d4(1, 1, 2024);
    Date d1_copy(1, 1, 2023);

    EXPECT_TRUE(d1 < d2);
    EXPECT_TRUE(d1 < d3);
    EXPECT_TRUE(d1 < d4);
    EXPECT_TRUE(d1 == d1_copy);
    EXPECT_TRUE(d2 > d1);
}


class WeatherDayTest : public ::testing::Test {
protected:
    WeatherDay createStandardDay(int t_morn, int t_day, int t_even, double precip, Phenomen phen) {
        Weather w1, w2, w3;
        w1.setTemperature(t_morn);
        w2.setTemperature(t_day);
        w3.setTemperature(t_even);
        
        PartsOfDay parts;
        parts.setMorning(w1);
        parts.setDay(w2);
        parts.setEvening(w3);
        return WeatherDay(Date(1,1,2000), precip, parts, static_cast<int>(phen));
    }
};

TEST_F(WeatherDayTest, AverageTemperature) {
    WeatherDay day = createStandardDay(10, 20, 30, 0.0, Phenomen::Sunny);
    EXPECT_EQ(day.averageTempOfDay(), 20);
    
    WeatherDay day2 = createStandardDay(10, 10, 11, 0.0, Phenomen::Sunny);
    EXPECT_EQ(day2.averageTempOfDay(), 10);
}

TEST_F(WeatherDayTest, CheckValidityRules) {
    auto good_day = createStandardDay(20, 25, 15, 0.0, Phenomen::Sunny);
    EXPECT_TRUE(good_day.check());

    auto bad_rain_sun = createStandardDay(20, 25, 15, 5.0, Phenomen::Sunny);
    EXPECT_FALSE(bad_rain_sun.check());

    auto too_hot = createStandardDay(61, 25, 15, 0.0, Phenomen::Sunny);
    EXPECT_FALSE(too_hot.check());
    
    auto too_cold = createStandardDay(-101, -50, -50, 0.0, Phenomen::Snowy);
    EXPECT_FALSE(too_cold.check());

    auto flood = createStandardDay(10, 10, 10, 1501.0, Phenomen::Rainy);
    EXPECT_FALSE(flood.check());

    auto warm_snow = createStandardDay(5, 10, 5, 10.0, Phenomen::Snowy);
    EXPECT_FALSE(warm_snow.check());
}

TEST_F(WeatherDayTest, OperatorPlusEqual) {
    WeatherDay d1 = createStandardDay(10, 10, 10, 10.0, Phenomen::Cloudy);
    WeatherDay d2 = createStandardDay(20, 20, 20, 20.0, Phenomen::Rainy);
    d1 += d2;

    EXPECT_EQ(d1.getPartsOfDay().getMorning().getTemperature(), 15);
    EXPECT_DOUBLE_EQ(d1.getPrecipitation(), 15.0);
    EXPECT_EQ(d1.getPhenomen(), Phenomen::Rainy);
}


class ForecastTest : public WeatherDayTest {
protected:
    Forecast f;
};

TEST_F(ForecastTest, AddDay) {
    WeatherDay d1 = createStandardDay(10, 10, 10, 0, Phenomen::Sunny);
    f += d1;
    EXPECT_EQ(f[0].getPrecipitation(), 0);
    EXPECT_THROW(f[1], std::out_of_range); 
}

TEST_F(ForecastTest, FindColdestDayInRange) {
    forecast_days_setup(f);

    Date start(1, 1, 2023);
    Date end(5, 1, 2023);
    
    WeatherDay coldest = f.findColdestDay(start, end);
    EXPECT_EQ(coldest.averageTempOfDay(), -20); 
}

TEST_F(ForecastTest, MergeDaysByData) {
    Weather w10; w10.setTemperature(10);
    PartsOfDay p10; p10.setMorning(w10); p10.setDay(w10); p10.setEvening(w10);
    WeatherDay d1(Date(1,1,2023), 0.0, p10, static_cast<int>(Phenomen::Sunny));
    Weather w20; w20.setTemperature(20);
    PartsOfDay p20; p20.setMorning(w20); p20.setDay(w20); p20.setEvening(w20);
    WeatherDay d2(Date(1,1,2023), 0.0, p20, static_cast<int>(Phenomen::Sunny));
    WeatherDay d3(Date(2,1,2023), 0.0, p10, static_cast<int>(Phenomen::Sunny));

    f += d1;
    f += d2;
    f += d3;

    f.mergeDaysByData();
    
    EXPECT_NO_THROW(f[0]);
    EXPECT_NO_THROW(f[1]);
    EXPECT_THROW(f[2], std::out_of_range);
    EXPECT_EQ(f[0].getDate().getDay(), 1);
    EXPECT_EQ(f[0].averageTempOfDay(), 16);
}

TEST_F(ForecastTest, LogicDeleteErrors) {
    f += createStandardDay(20, 20, 20, 0, Phenomen::Sunny);
    f += createStandardDay(20, 20, 20, 2000, Phenomen::Rainy);
    f.deleteAllErrors();
    
    EXPECT_NO_THROW(f[0]);
    EXPECT_THROW(f[1], std::out_of_range);
    EXPECT_EQ(f[0].getPrecipitation(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
