// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdlib.h>
#include <string>
#include <string_view>

// my clang format is messed up right now so sorry for the terrible formatting

struct CubeData {
  int red = 0;
  int green = 0;
  int blue = 0;
  friend auto operator<=>(const CubeData &, const CubeData &) = default;
  int Power() { return red * green * blue; }
};

struct GameData {
  int id;
  std::vector<CubeData> cubeData;
};

GameData ParseAGame(const std::string &line);
bool GameIsPossible(const GameData &game, const CubeData &maxOfEachCube);
CubeData GetMinCubesToPlayGame(const GameData &game);
int Solve(const std::vector<std::string> &lines, const CubeData &maxOfEachCube);
int SolvePart2(const std::vector<std::string> &lines);
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

TEST_CASE("Solve part 1 simple", "[Day2]") {
  CubeData maxOfEachCube{12, 13, 14};
  REQUIRE(8 == Solve(
                   std::vector<std::string>{
                       "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green",
                       "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 "
                       "green, 1 blue",
                       "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 "
                       "green; 5 green, "
                       "1 red",
                       "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 "
                       "green, 15 blue, "
                       "14 red",
                       "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green",
                   },
                   maxOfEachCube));
}

TEST_CASE("Solve part 1 from file", "[Day2]") {
  CubeData maxOfEachCube{12, 13, 14};
  REQUIRE(2913 == Solve(GetPuzzleInput(), maxOfEachCube));
}

TEST_CASE("GetMinCubesToPlayGame", "[Day2]") {
  REQUIRE(CubeData{4, 2, 6} ==
          GetMinCubesToPlayGame(ParseAGame(
              "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green")));
  REQUIRE(
      CubeData{1, 3, 4} ==
      GetMinCubesToPlayGame(ParseAGame(
          "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue")));
  REQUIRE(CubeData{20, 13, 6} == GetMinCubesToPlayGame(ParseAGame(
                                     "Game 3: 8 green, 6 blue, 20 red; 5 blue, "
                                     "4 red, 13 green; 5 green, 1 red")));
  REQUIRE(CubeData{14, 3, 15} == GetMinCubesToPlayGame(ParseAGame(
                                     "Game 4: 1 green, 3 red, 6 blue; 3 green, "
                                     "6 red; 3 green, 15 blue, 14 red")));
  REQUIRE(CubeData{6, 3, 2} ==
          GetMinCubesToPlayGame(ParseAGame(
              "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green")));
}

TEST_CASE("Solve part 2 simple", "[Day2]") {
  REQUIRE(2286 == SolvePart2(std::vector<std::string>{
                      "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green",
                      "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 "
                      "green, 1 blue",
                      "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 "
                      "green; 5 green, "
                      "1 red",
                      "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 "
                      "green, 15 blue, "
                      "14 red",
                      "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green",
                  }));
}

TEST_CASE("Solve part 2 from file", "[Day2]") {
  REQUIRE(55593 == SolvePart2(GetPuzzleInput()));
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

CubeData GetMinCubesToPlayGame(const GameData &game) {
  CubeData result;
  for (const auto &set : game.cubeData) {
    result.red = std::max(result.red, set.red);
    result.green = std::max(result.green, set.green);
    result.blue = std::max(result.blue, set.blue);
  }
  return result;
}

int Solve(const std::vector<std::string> &lines,
          const CubeData &maxOfEachCube) {
  int result = 0;
  for (const auto &line : lines) {
    auto game{ParseAGame(line)};
    if (GameIsPossible(game, maxOfEachCube)) {
      result += game.id;
    }
  }
  return result;
}

int SolvePart2(const std::vector<std::string> &lines) {
  int result{0};
  for (const auto &line : lines) {
    auto game{ParseAGame(line)};
    auto minCubes = GetMinCubesToPlayGame(game);
    result += minCubes.Power();
  }
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