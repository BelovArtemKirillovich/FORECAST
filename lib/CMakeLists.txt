add_library(weather_lib STATIC 
    src/weather.cpp src/weather_day.cpp src/date.cpp src/parts_of_day.cpp src/forecast.cpp
)

target_include_directories(weather_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_COVERAGE)
    target_compile_options(weather_lib PRIVATE --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(weather_lib PRIVATE --coverage)
endif()
