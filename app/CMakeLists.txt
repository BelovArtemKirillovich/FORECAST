# Исполняемый файл приложения
add_executable(app_main src/main.cpp)
target_link_libraries(app_main weather_lib)
target_include_directories(app_main PRIVATE ${CMAKE_SOURCE_DIR}/include)