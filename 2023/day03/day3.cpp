// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>

std::tuple<bool, int, int> IdentifyNumber(char *in);
bool SymbolAdjacent(const std::vector<std::string> &search, int row, int column,
                    int width);
int Solve(const std::vector<std::string> &lines);
std::vector<std::string> GetPuzzleInput();

TEST_CASE("symbolAdjacent", "[day3]") {
  REQUIRE(SymbolAdjacent(std::vector<std::string>{"24@"}, 0, 0, 2));
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

TEST_CASE("part 1 simple", "[day3]") {
  REQUIRE(4361 == Solve(std::vector<std::string>{
                      "467..114..", "...*......", "..35..633.", "......#...",
                      "617*......", ".....+.58.", "..592.....", "......755.",
                      "...$.*....", ".664.598.."}));
}

bool SymbolAdjacent(const std::vector<std::string> &search, int row, int column,
                    int width) {}

std::tuple<bool, int, int> IdentifyNumber(char *in) {
  if (nullptr == in) {
    throw "Why would you do that";
  }
  int possibleI = std::atoi(in);
  if (0 == possibleI && '0' != *in) {
    return {false, 0, 0};
  }
  return {true, possibleI, static_cast<int>(std::to_string(possibleI).size())};
}

int Solve(const std::vector<std::string> &lines) {
  int result = 0;
  return result;
}

std::vector<std::string> GetPuzzleInput() {
  // build is down one folder
  std::ifstream file("../puzzle_in.txt");
  if (!file.is_open()) {
    throw "I can't get the file give up";
  }
  std::string line;
  std::vector<std::string> result;
  while (std::getline(file, line)) {
    result.push_back(line);
  }
  file.close();
  return result;
}