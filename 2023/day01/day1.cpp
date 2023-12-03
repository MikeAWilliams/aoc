// main.cpp
#include <string>
#include <catch2/catch_test_macros.hpp>
#include <tuple>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>

std::tuple<int,int> GetDigits(const std::string& line);
int GetIntValueOfDigit(const char digitChar);
int ConvertDigitsToLineValue(std::tuple<int,int> digits);
int Solve(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();

TEST_CASE( "GetDigits", "[day1]" )
{
    auto [first, last] = GetDigits("1abc2");
    REQUIRE(1 == first);
    REQUIRE(2 == last);

    std::tie(first, last) = GetDigits("pqr3stu8vwx");
    REQUIRE(3 == first);
    REQUIRE(8 == last);

    std::tie(first, last) = GetDigits("a1b2c3d4e5f");
    REQUIRE(1 == first);
    REQUIRE(5 == last);

    std::tie(first, last) = GetDigits("treb7uchet");
    REQUIRE(7 == first);
    REQUIRE(7 == last);
}

TEST_CASE( "GetLineValue", "[day1]" )
{
    REQUIRE(12 == ConvertDigitsToLineValue(std::tuple(1,2)));
    REQUIRE(38 == ConvertDigitsToLineValue(std::tuple(3,8)));
    REQUIRE(15 == ConvertDigitsToLineValue(std::tuple(1,5)));
    REQUIRE(77 == ConvertDigitsToLineValue(std::tuple(7,7)));
}

TEST_CASE( "Solve Part 1 Simple", "[day1]" )
{
    std::vector<std::string> data {
        "1abc2",
        "pqr3stu8vwx",
        "a1b2c3d4e5f",
        "treb7uchet"
    };
    REQUIRE(142 == Solve(data));
}

TEST_CASE( "Solve Part 1", "[day1]" )
{
    REQUIRE(55123 == Solve(GetPuzzleInput()));
}


std::tuple<int,int> GetDigits(const std::string& line)
{
    auto first = std::find_if(std::cbegin(line), std::cend(line), [](const auto &c){
        return std::isdigit(static_cast<int>(c));
    });
    if(std::cend(line) == first){
        throw "can't find first character give up";
    }

    auto last = std::find_if(std::crbegin(line), std::crend(line), [](const auto &c){
        return std::isdigit(static_cast<int>(c));
    });
    if(std::crend(line) == last){
        throw "can't find last character give up";
    }

    return std::tuple(GetIntValueOfDigit(*first), GetIntValueOfDigit(*last));
}

int GetIntValueOfDigit(const char digitChar)
{
    // super grose
    return static_cast<int>(digitChar) - 48;
}

int ConvertDigitsToLineValue(std::tuple<int,int> digits)
{
    return std::get<0>(digits) * 10 + std::get<1>(digits);
}

int Solve(const std::vector<std::string>& lines)
{
    int result = 0;
    for (const auto & line : lines)
    {
        result += ConvertDigitsToLineValue(GetDigits(line));
    }
    return result;
}

std::vector<std::string> GetPuzzleInput()
{
    // build is down one folder
    std::ifstream file("../puzzle_in.txt");
    if (!file.is_open()) 
    {
        throw "I can't get the file give up";
    }
    std::string line;
    std::vector<std::string> result;
    while (std::getline(file, line))
    {
        result.push_back(line);
    }
    file.close();
    return result;
}