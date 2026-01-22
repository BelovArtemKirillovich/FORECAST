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

TEST(WeatherTest, TemperatureValidation) {
    Weather w;
    EXPECT_NO_THROW(w.setTemperature(-273));
    EXPECT_THROW(w.setTemperature(-274), std::invalid_argument);
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

TEST_F(ForecastTest, CopyAssignment) {
    f += createStandardDay(10, 10, 10, 0, Phenomen::Sunny);
    Forecast f2;
    f2 = f;
    EXPECT_EQ(f2[0].getDate(), f[0].getDate());
    f = f;
    EXPECT_EQ(f[0].getDate().getDay(), 1);
}

TEST_F(ForecastTest, MoveSemantics) {
    f += createStandardDay(10, 10, 10, 0, Phenomen::Sunny);
    Forecast f_moved(std::move(f));
    EXPECT_NO_THROW(f_moved[0]);
    EXPECT_THROW(f[0], std::out_of_range);
    Forecast f3;
    f3 = std::move(f_moved);
    EXPECT_NO_THROW(f3[0]);
}

TEST_F(ForecastTest, AutoResize) {
    for(int i = 0; i < 50; ++i) {
        Weather w; w.setTemperature(i); 
        PartsOfDay p; p.setMorning(w); p.setDay(w); p.setEvening(w);
        f += WeatherDay(Date(1, 1, 2000 + i), 0, p, static_cast<int>(Phenomen::Sunny));
    }
    EXPECT_NO_THROW(f[49]);
    EXPECT_EQ(f[49].averageTempOfDay(), 49);
}

TEST_F(WeatherDayTest, PhenomenLogic) {
    Weather w_cold; w_cold.setTemperature(-10);
    PartsOfDay p_cold; p_cold.setMorning(w_cold); p_cold.setDay(w_cold); p_cold.setEvening(w_cold);
    WeatherDay snow_day(Date(1,1,2000), 5.0, p_cold);
    EXPECT_EQ(snow_day.getPhenomen(), Phenomen::Snowy);
    Weather w_warm; w_warm.setTemperature(10);
    PartsOfDay p_warm; p_warm.setMorning(w_warm); p_warm.setDay(w_warm); p_warm.setEvening(w_warm);
    WeatherDay rain_day(Date(1,1,2000), 5.0, p_warm);
    EXPECT_EQ(rain_day.getPhenomen(), Phenomen::Rainy);
    WeatherDay dry_day(Date(1,1,2000), 0.0, p_warm);
    EXPECT_NE(dry_day.getPhenomen(), Phenomen::Rainy);
    EXPECT_NE(dry_day.getPhenomen(), Phenomen::Snowy);
}

TEST_F(ForecastTest, FindNextSunnyDay) {
    Weather w; w.setTemperature(10);
    PartsOfDay p; p.setMorning(w); p.setDay(w); p.setEvening(w);
    f += WeatherDay(Date(1,1,2023), 5.0, p, static_cast<int>(Phenomen::Rainy));
    f += WeatherDay(Date(5,1,2023), 0.0, p, static_cast<int>(Phenomen::Sunny));
    WeatherDay res = f.findNextSunnyDay(Date(2,1,2023));
    EXPECT_EQ(res.getDate().getDay(), 5);
    EXPECT_THROW(f.findNextSunnyDay(Date(6,1,2023)), std::runtime_error);
}

TEST_F(ForecastTest, GiveAllDaysOfMonth) {
    Weather w; w.setTemperature(10);
    PartsOfDay p; p.setMorning(w); p.setDay(w); p.setEvening(w);
    f += WeatherDay(Date(1,1,2023), 0.0, p, static_cast<int>(Phenomen::Sunny));
    f += WeatherDay(Date(1,2,2023), 0.0, p, static_cast<int>(Phenomen::Sunny)); 
    
    Forecast jan = f.giveAllDaysOfMonth(1);
    EXPECT_NO_THROW(jan[0]);
    EXPECT_THROW(jan[1], std::out_of_range);
    EXPECT_EQ(jan[0].getDate().getMonth(), 1);
}

TEST(IOTest, DateOutputInput) {
    Date d(15, 10, 2023);
    std::stringstream ss;
    d.print();
    std::stringstream input_ss("01.05.2023");
    Date d_in;
    input_ss >> d_in;
    EXPECT_EQ(d_in.getDay(), 1);
    EXPECT_EQ(d_in.getMonth(), 5);
    EXPECT_EQ(d_in.getYear(), 2023);
    std::stringstream bad_ss("invalid");
    Date d_bad;
    bad_ss >> d_bad;
    EXPECT_TRUE(bad_ss.fail());
}

TEST(IOTest, WeatherDayOutput) {
    Weather w; w.setTemperature(10);
    PartsOfDay p; p.setMorning(w); p.setDay(w); p.setEvening(w);
    WeatherDay day(Date(1,1,2000), 10.0, p, static_cast<int>(Phenomen::Rainy));
    std::stringstream ss;
    ss << day;
    std::string output = ss.str();
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("RAINY") != std::string::npos);
}

TEST(ForecastMemoryTest, Constructors) {
    Forecast f_def;
    EXPECT_THROW(f_def[0], std::out_of_range);
    Forecast f_cap(10);
    EXPECT_THROW(f_cap[0], std::out_of_range); 
    Weather w; w.setTemperature(10);
    PartsOfDay p; p.setMorning(w); p.setDay(w); p.setEvening(w);
    WeatherDay raw_arr[2];
    raw_arr[0] = WeatherDay(Date(1,1,2023), 0, p, static_cast<int>(Phenomen::Sunny));
    raw_arr[1] = WeatherDay(Date(2,1,2023), 0, p, static_cast<int>(Phenomen::Cloudy));
    Forecast f_arr(raw_arr, 2);
    EXPECT_NO_THROW(f_arr[1]);
    EXPECT_EQ(f_arr[0].getDate().getDay(), 1);
    EXPECT_EQ(f_arr[1].getDate().getDay(), 2);
}

TEST_F(ForecastTest, CopyConstructor) {
    f += createStandardDay(10, 10, 10, 0, Phenomen::Sunny); 
    f += createStandardDay(20, 20, 20, 0, Phenomen::Rainy); 
    Forecast f_copy(f); 
    EXPECT_EQ(f_copy[0].getDate(), f[0].getDate());
    EXPECT_EQ(f_copy[0].getPhenomen(), f[0].getPhenomen());
    f_copy.deleteByIndex(0);
    EXPECT_NO_THROW(f[0]);
    EXPECT_THROW(f_copy[1], std::out_of_range);
}

TEST_F(ForecastTest, CopyAssignmentOperator) {
    f += createStandardDay(10, 10, 10, 0, Phenomen::Sunny);
    Forecast f2;
    f2 += createStandardDay(-100, -100, -100, 0, Phenomen::Snowy);
    f2 = f;
    EXPECT_EQ(f2[0].averageTempOfDay(), 10);
    f = f;
    EXPECT_NO_THROW(f[0]);
    EXPECT_EQ(f[0].averageTempOfDay(), 10);
}

TEST_F(ForecastTest, MoveConstructor) {
    f += createStandardDay(15, 15, 15, 5, Phenomen::Cloudy);
    Forecast f_moved(std::move(f));
    EXPECT_NO_THROW(f_moved[0]);
    EXPECT_EQ(f_moved[0].averageTempOfDay(), 15);
    EXPECT_THROW(f[0], std::out_of_range);
}

TEST_F(ForecastTest, MoveAssignmentOperator) {
    f += createStandardDay(30, 30, 30, 0, Phenomen::Sunny);
    Forecast f_target;
    f_target = std::move(f);
    EXPECT_EQ(f_target[0].averageTempOfDay(), 30);
    EXPECT_THROW(f[0], std::out_of_range);
    f_target = std::move(f_target);
    EXPECT_NO_THROW(f_target[0]); 
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
