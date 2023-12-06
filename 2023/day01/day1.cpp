// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>

std::tuple<int, int>     GetDigits(const std::string& line);
int                      GetIntValueOfDigit(const char digitChar);
int                      ConvertDigitsToLineValue(std::tuple<int, int> digits);
int                      Solve(const std::vector<std::string>& lines);
int                      SolveWithWords(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();
std::string              SubsituteDigitsForWords(const std::string& input);

TEST_CASE("GetDigits", "[day1]") {
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

TEST_CASE("GetLineValue", "[day1]") {
    REQUIRE(12 == ConvertDigitsToLineValue(std::tuple(1, 2)));
    REQUIRE(38 == ConvertDigitsToLineValue(std::tuple(3, 8)));
    REQUIRE(15 == ConvertDigitsToLineValue(std::tuple(1, 5)));
    REQUIRE(77 == ConvertDigitsToLineValue(std::tuple(7, 7)));
}

TEST_CASE("Solve Part 1 Simple", "[day1]") {
    std::vector<std::string> data{
        "1abc2", "pqr3stu8vwx", "a1b2c3d4e5f", "treb7uchet"};
    REQUIRE(142 == Solve(data));
}

TEST_CASE("Solve Part 1", "[day1]") {
    REQUIRE(55123 == Solve(GetPuzzleInput()));
}

TEST_CASE("Solve Part 2 simple", "[day1]") {
    std::vector<std::string> data{
        "two1nine",         "eightwothree", "abcone2threexyz", "xtwone3four",
        "4nineeightseven2", "zoneight234",  "7pqrstsixteen"};
    REQUIRE(281 == SolveWithWords(data));
}

TEST_CASE("Solve Part 2", "[day1]") {
    REQUIRE(55260 == SolveWithWords(GetPuzzleInput()));
}

std::tuple<int, int> GetDigits(const std::string& line) {
    auto first =
        std::find_if(std::cbegin(line), std::cend(line), [](const auto& c) {
            return std::isdigit(static_cast<int>(c));
        });
    if (std::cend(line) == first) {
        std::cout << "\n\n loook here \n\n" << line << "\n\n\n";
        throw "can't find first character give up";
    }

    auto last =
        std::find_if(std::crbegin(line), std::crend(line), [](const auto& c) {
            return std::isdigit(static_cast<int>(c));
        });
    if (std::crend(line) == last) {
        throw "can't find last character give up";
    }

    return std::tuple(GetIntValueOfDigit(*first), GetIntValueOfDigit(*last));
}

int GetIntValueOfDigit(const char digitChar) {
    // super grose
    return static_cast<int>(digitChar) - 48;
}

int ConvertDigitsToLineValue(std::tuple<int, int> digits) {
    return std::get<0>(digits) * 10 + std::get<1>(digits);
}

int Solve(const std::vector<std::string>& lines) {
    int result = 0;
    for (const auto& line : lines) {
        result += ConvertDigitsToLineValue(GetDigits(line));
    }
    return result;
}

std::vector<std::string> GetPuzzleInput() {
    // build is down one folder
    std::ifstream file("../puzzle_in.txt");
    if (!file.is_open()) {
        throw "I can't get the file give up";
    }
    std::string              line;
    std::vector<std::string> result;
    while (std::getline(file, line)) {
        result.push_back(line);
    }
    file.close();
    return result;
}

// Contract: Mutate input by replacing the leftmost digit word with its digit
//           Return true if a change was made
//           I hate this but it worked it is so wastefull
bool ReplaceLeftMostDigitWord(std::string& input) {
    std::vector<std::pair<std::string, std::string>> replaceKey{
        {"one", "1"},   {"two", "2"},   {"three", "3"},
        {"four", "4"},  {"five", "5"},  {"six", "6"},
        {"seven", "7"}, {"eight", "8"}, {"nine", "9"}};

    int minIndex = static_cast<int>(input.length());
    std::pair<std::string, std::string> minPair;
    for (const auto& pair : replaceKey) {
        int index = input.find(pair.first);
        if (index != std::string::npos) {
            if (index < minIndex) {
                minIndex = index;
                minPair  = pair;
            }
        }
    }
    if (minIndex < static_cast<int>(input.length())) {
        // input.replace(minIndex, minPair.first.size(), minPair.second);
        input.replace(minIndex, 1, minPair.second);
        return true;
    }
    return false;
}

std::string SubsituteDigitsForWords(const std::string& input) {
    std::string result{input};
    auto        didReplace = ReplaceLeftMostDigitWord(result);
    while (didReplace) {
        didReplace = ReplaceLeftMostDigitWord(result);
    }
    return result;
}

int SolveWithWords(const std::vector<std::string>& lines) {
    std::vector<std::string> withDigitsForWords;
    withDigitsForWords.reserve(lines.size());
    std::transform(
        std::cbegin(lines), std::cend(lines),
        std::back_insert_iterator(withDigitsForWords),
        [](auto line) { return SubsituteDigitsForWords(line); });
    return Solve(withDigitsForWords);
}