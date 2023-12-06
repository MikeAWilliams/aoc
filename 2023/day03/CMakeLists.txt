# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)

project(test)

find_package(Catch2 3 REQUIRED)

add_executable(day3 day3.cpp)

target_link_libraries(day3 PRIVATE Catch2::Catch2WithMain)
target_compile_features(day3 PUBLIC cxx_std_23)