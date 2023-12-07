// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>

std::tuple<bool, int, int> IdentifyNumber(const char* in);
bool                       SymbolAdjacent(
                          const std::vector<std::string>& search, int row, int column, int width);
int                      Solve(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();

TEST_CASE("symbolAdjacent", "[day3]") {
    // no symbol
    REQUIRE_FALSE(SymbolAdjacent(std::vector<std::string>{"4"}, 0, 0, 1));
    // dots are not symbol
    REQUIRE_FALSE(SymbolAdjacent(std::vector<std::string>{".4."}, 0, 0, 1));
    // symbol at +1 col
    REQUIRE(SymbolAdjacent(std::vector<std::string>{"4@"}, 0, 0, 1));
    // symbol at +1 col longer than 1
    REQUIRE(SymbolAdjacent(std::vector<std::string>{"444@"}, 0, 0, 3));
    // symbol at -1 col
    REQUIRE(SymbolAdjacent(std::vector<std::string>{"@4"}, 0, 1, 1));
    // symbol at -1 row
    REQUIRE(SymbolAdjacent(
        std::vector<std::string>{
            // clang-format off
      ".@.",
      ".4.",
            // clang-format on
        },
        1, 1, 1));
    // symbol at +1 row
    REQUIRE(SymbolAdjacent(
        std::vector<std::string>{
            // clang-format off
      ".4.",
      ".@.",
            // clang-format on
        },
        0, 1, 1));
    // symbol at +1 col +1 row
    REQUIRE(SymbolAdjacent(
        std::vector<std::string>{
            // clang-format off
      ".4.",
      "..@",
            // clang-format on
        },
        0, 1, 1));
    // symbol at -1 col +1 row
    REQUIRE(SymbolAdjacent(
        std::vector<std::string>{
            // clang-format off
      ".4.",
      "@..",
            // clang-format on
        },
        0, 1, 1));
    // symbol at -1 row -1 col
    REQUIRE(SymbolAdjacent(
        std::vector<std::string>{
            // clang-format off
      "@..",
      ".4.",
            // clang-format on
        },
        1, 1, 1));
    // symbol at -1 row +1 col
    REQUIRE(SymbolAdjacent(
        std::vector<std::string>{
            // clang-format off
      "..@",
      ".4.",
            // clang-format on
        },
        1, 1, 1));

    // some random larger than 1 tests
    // symbol at +1 col +1 row
    REQUIRE(SymbolAdjacent(
        std::vector<std::string>{
            // clang-format off
      ".444.",
      "..@..",
            // clang-format on
        },
        0, 1, 1));
}
TEST_CASE("IdentifyNumber", "[day3]") {
    auto [isInt, intResult, intLength] =
        IdentifyNumber(std::string("467.").data());
    REQUIRE(isInt);
    REQUIRE(467 == intResult);
    REQUIRE(3 == intLength);

    std::tie(isInt, intResult, intLength) =
        IdentifyNumber(std::string("467%").data());
    REQUIRE(isInt);
    REQUIRE(467 == intResult);
    REQUIRE(3 == intLength);

    std::tie(isInt, intResult, intLength) =
        IdentifyNumber(std::string(".").data());
    REQUIRE_FALSE(isInt);

    std::tie(isInt, intResult, intLength) =
        IdentifyNumber(std::string("").data());
    REQUIRE_FALSE(isInt);

    std::tie(isInt, intResult, intLength) =
        IdentifyNumber(std::string(".3").data());
    REQUIRE_FALSE(isInt);
}

TEST_CASE("part 1 mike", "[day3]") {
    REQUIRE(
        467 + 114 == Solve(std::vector<std::string>{
                         // clang-format off
            "467.114*..",
            "...*......",
    }));
    // clang-format on
}

TEST_CASE("part 1 simple", "[day3]") {
    REQUIRE(
        4361 == Solve(std::vector<std::string>{
                    // clang-format off
            "467..114..",
            "...*......",
            "..35..633.",
            "......#...",
            "617*......",
            ".....+.58.",
            "..592.....",
            "......755.",
            "...$.*....",
            ".664.598.."
    }));
    // clang-format on
}

TEST_CASE("part 1 from file", "[day3]") {
    REQUIRE(514969 == Solve(GetPuzzleInput()));
}

bool SymbolAdjacent(
    const std::vector<std::string>& search, int row, int column, int width) {
    if (row >= search.size() || column >= search[row].length()) {
        throw "index out of bounds";
    }
    // pairs of {column offset, row offset}
    // all thse count as adjacent
    const std::vector<std::pair<int, int>> adjacency{
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1},
    };

    for (int i = column; i < column + width; ++i) {
        for (const auto& offset : adjacency) {
            auto check_row = row + offset.second;
            if (check_row >= search.size()) {
                continue;
            }
            auto check_column = i + offset.first;
            if (check_column >= search[row + offset.second].length()) {
                continue;
            }
            auto check_char = search[check_row][check_column];
            if (std::isdigit(static_cast<int>(check_char)) ||
                '.' == check_char) {
                continue;
            }
            return true;
        }
    }
    return false;
}

std::tuple<bool, int, int> IdentifyNumber(const char* in) {
    if (nullptr == in) {
        throw "Why would you do that";
    }
    int possibleI = std::atoi(in);
    if (0 == possibleI && '0' != *in) {
        return {false, 0, 0};
    }
    return {
        true, possibleI, static_cast<int>(std::to_string(possibleI).size())};
}

int Solve(const std::vector<std::string>& lines) {
    int result = 0;

    for (int rowIndex = 0; rowIndex < static_cast<int>(lines.size());
         ++rowIndex) {
        for (int columnIndex = 0; columnIndex < lines[rowIndex].size();
             ++columnIndex) {
            if ('-' == lines[rowIndex][columnIndex]) {
                // the author of this puzzle meant to exclude negative numbers
                // but atoi will find -357 as a negative number
                continue;
            }
            auto [isNumber, value, length] =
                IdentifyNumber(lines[rowIndex].data() + columnIndex);
            if (isNumber) {
                // std::cout << value << " " << length << std::endl;
                if (SymbolAdjacent(lines, rowIndex, columnIndex, length)) {
                    result += value;
                }
                columnIndex += length;
            }
        }
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