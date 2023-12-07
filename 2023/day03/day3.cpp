// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>

const std::vector<std::pair<int, int>> ADJACENT_OFFSETS{
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1},
};

struct PartNumber {
    int  rowIndex, columnIndex;
    int  value, lenghth;
    bool Intersects(int row, int column) const;
};

std::tuple<bool, int, int> IdentifyNumber(const char* in);
bool                       SymbolAdjacent(
                          const std::vector<std::string>& search, int row, int column, int width);
std::vector<PartNumber>  FindPartNumbers(const std::vector<std::string>& lines);
int                      Solve(const std::vector<std::string>& lines);
int                      SolvePart2(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();

TEST_CASE("PartNumber Intersects", "[day3]") {
    PartNumber testObject{10, 10, 100, 4};
    // not in the right row
    REQUIRE_FALSE(testObject.Intersects(9, 10));
    REQUIRE_FALSE(testObject.Intersects(11, 10));
    // not in the right column
    REQUIRE_FALSE(testObject.Intersects(10, 9));
    REQUIRE_FALSE(testObject.Intersects(10, 14));

    REQUIRE(testObject.Intersects(10, 10));
    REQUIRE(testObject.Intersects(10, 11));
    REQUIRE(testObject.Intersects(10, 12));
    REQUIRE(testObject.Intersects(10, 13));
}

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

TEST_CASE("part 2 simple", "[day3]") {
    REQUIRE(
        467835 == SolvePart2(std::vector<std::string>{
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

TEST_CASE("part 2 from file", "[day3]") {
    REQUIRE(78915902 == SolvePart2(GetPuzzleInput()));
}

bool SymbolAdjacent(
    const std::vector<std::string>& search, int row, int column, int width) {
    if (row >= search.size() || column >= search[row].length()) {
        throw "index out of bounds";
    }
    // pairs of {column offset, row offset}
    // all thse count as adjacent

    for (int i = column; i < column + width; ++i) {
        for (const auto& offset : ADJACENT_OFFSETS) {
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
    auto partNumbers{FindPartNumbers(lines)};
    return std::accumulate(
        std::cbegin(partNumbers), std::cend(partNumbers), 0,
        [](auto runningTotal, auto part) { return runningTotal + part.value; });
}

int SolvePart2(const std::vector<std::string>& lines) {
    auto result = 0;
    auto partNumbers{FindPartNumbers(lines)};
    for (int rowIndex = 0; rowIndex < static_cast<int>(lines.size());
         ++rowIndex) {
        for (int columnIndex = 0; columnIndex < lines[rowIndex].size();
             ++columnIndex) {
            if ('*' != lines[rowIndex][columnIndex]) {
                continue;
            }
            auto adjacentParts = partNumbers | std::views::filter([rowIndex, columnIndex](const auto& part){
                return !(ADJACENT_OFFSETS | std::views::filter([rowIndex, columnIndex, &part](const auto& offset){
                    return part.Intersects(rowIndex + offset.first, columnIndex + offset.second);
                })).empty();
            });
            // i'm pretty sure i'm using rangees wrong here becasue I want adjacentParts.size() but its not there. Also no support for const filter result or cbegin cend
            if(2 == std::distance(std::begin(adjacentParts), std::end(adjacentParts)))
            {
                // fold_left is in c++23 but not until gcc13 and i'm on 12 
                // not sure if this is correct syntax because I can't compile it.
                //int gearRatio = std::ranges::fold_left(adjacentParts, 1, [](int valueSoFar, const auto& partNumber){
                   //return valueSoFar * partNumber.value; 
                //});
                // lets use a silly for loop. who needs ranges anyway
                int gearRatio = 1;
                for(const auto& partNumber : adjacentParts){
                    gearRatio *= partNumber.value;
                }
                result += gearRatio;
            }
        }
    }
    return result;
}

std::vector<PartNumber> FindPartNumbers(const std::vector<std::string>& lines) {
    std::vector<PartNumber> result;

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
                    result.push_back({rowIndex, columnIndex, value, length});
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

bool PartNumber::Intersects(int row, int column) const {
    if (this->rowIndex != row) {
        return false;
    }
    return column >= this->columnIndex &&
           column < this->columnIndex + this->lenghth;
}