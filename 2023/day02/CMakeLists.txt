# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)

project(test)

find_package(Catch2 3 REQUIRED)

add_executable(day2 day2.cpp)

target_link_libraries(day2 PRIVATE Catch2::Catch2WithMain)
target_compile_features(day2 PUBLIC cxx_std_23)