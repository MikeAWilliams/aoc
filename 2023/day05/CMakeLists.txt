# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)

project(test)

find_package(Catch2 3 REQUIRED)
find_package(TBB REQUIRED)

add_executable(day5 day5.cpp)

add_compile_options(-O3)
target_link_libraries(day5 PRIVATE Catch2::Catch2WithMain TBB::tbb)
target_compile_features(day5 PUBLIC cxx_std_23)