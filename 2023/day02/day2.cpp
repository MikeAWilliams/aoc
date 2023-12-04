// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdlib.h>
#include <string>
#include <string_view>

struct CubeData {
  int red = 0;
  int green = 0;
  int blue = 0;
  friend auto operator<=>(const CubeData &, const CubeData &) = default;
};

struct GameData {
  int id;
  std::vector<CubeData> cubeData;
};

GameData ParseAGame(const std::string &line);
bool GameIsPossible(const GameData &game, const CubeData &maxOfEachCube);
int Solve(const std::vector<std::string> &lines);
std::vector<std::string> GetPuzzleInput();

TEST_CASE("ParseAGame", "[day2]") {
  auto result =
      ParseAGame("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green");
  REQUIRE(1 == result.id);
  REQUIRE(3 == result.cubeData.size());
  REQUIRE(CubeData{4, 0, 3} == result.cubeData[0]);
  REQUIRE(CubeData{1, 2, 6} == result.cubeData[1]);
  REQUIRE(CubeData{0, 2, 0} == result.cubeData[2]);

  result =
      ParseAGame("Game 100: 1 blue, 1 red, 1 green; 8 blue, 1 green; 1 green, "
                 "7 blue, 1 red; 1 green, 4 blue, 1 red; 1 green, 3 blue ");
  REQUIRE(100 == result.id);
  REQUIRE(5 == result.cubeData.size());
  REQUIRE(CubeData{1, 1, 1} == result.cubeData[0]);
  REQUIRE(CubeData{0, 1, 8} == result.cubeData[1]);
  REQUIRE(CubeData{1, 1, 7} == result.cubeData[2]);
  REQUIRE(CubeData{1, 1, 4} == result.cubeData[3]);
  REQUIRE(CubeData{0, 1, 3} == result.cubeData[4]);
}

TEST_CASE("GameIsPossible", "[day2]") {
  CubeData maxOfEachCube{12, 13, 14};
  REQUIRE(GameIsPossible(
      ParseAGame("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"),
      maxOfEachCube));
  REQUIRE(GameIsPossible(
      ParseAGame(
          "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue"),
      maxOfEachCube));
  REQUIRE_FALSE(
      GameIsPossible(ParseAGame("Game 3: 8 green, 6 blue, 20 red; 5 blue, "
                                "4 red, 13 green; 5 green, 1 red"),
                     maxOfEachCube));
  REQUIRE_FALSE(
      GameIsPossible(ParseAGame("Game 4: 1 green, 3 red, 6 blue; 3 green, "
                                "6 red; 3 green, 15 blue, 14 red"),
                     maxOfEachCube));
  REQUIRE(GameIsPossible(
      ParseAGame("Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"),
      maxOfEachCube));
}

GameData ParseAGame(const std::string &line) {
  GameData result{};
  auto firstSpace = line.find(' ');
  if (firstSpace == std::string::npos) {
    throw "Invalid game no space";
  }
  auto colon = line.find(':');
  if (colon == std::string::npos) {
    throw "Invalid game no colon";
  }
  result.id = std::stoi(line.substr(firstSpace, colon - firstSpace));

  auto gamesString = line.substr(colon + 1);
  // ranges!
  for (const auto game : std::views::split(gamesString, ';')) {
    CubeData thisSet;
    for (const auto set : std::views::split(game, ',')) {
      std::string_view setView{set};
      auto colorSplit = setView.rfind(' ');
      if (std::string::npos == colorSplit) {
        throw "error splitting a cube color";
      }
      auto color = setView.substr(colorSplit + 1);
      auto num = setView.substr(1, colorSplit);
      if ("red" == color) {
        thisSet.red = atoi(num.data());
      } else if ("green" == color) {
        thisSet.green = atoi(num.data());
      } else {
        thisSet.blue = atoi(num.data());
      }
    }
    result.cubeData.push_back(thisSet);
  }

  return result;
}

bool GameIsPossible(const GameData &game, const CubeData &maxOfEachCube) {
  for (const auto &set : game.cubeData) {
    if (set.blue > maxOfEachCube.blue || set.green > maxOfEachCube.green ||
        set.red > maxOfEachCube.red) {
      return false;
    }
  }
  return true;
}

int Solve(const std::vector<std::string> &lines) { return 0; }

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