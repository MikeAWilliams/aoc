// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <compare>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

struct RaceRecord {
    int64_t time;
    int64_t distance;
};

struct RaceResult {
    int  buttonTime;
    int  raceDistance;
    auto operator<=>(const RaceResult&) const = default;
};

std::vector<RaceResult> FindPossibleRaces(const RaceRecord& record);
int                     CountWinningRaces(
                        const RaceRecord& record, const std::vector<RaceResult>& results);
int     SolvePart1(std::vector<RaceRecord>& puzzleInput);
int64_t SolvePart2(const RaceRecord& input);

TEST_CASE("FindPossibleRaces", "[day6]") {
    auto result = FindPossibleRaces({7, 9});
    REQUIRE(8 == result.size());
    REQUIRE(result[0] == RaceResult{0, 0});
    REQUIRE(result[1] == RaceResult{1, 6});
    REQUIRE(result[2] == RaceResult{2, 10});
    REQUIRE(result[3] == RaceResult{3, 12});
    REQUIRE(result[4] == RaceResult{4, 12});
    REQUIRE(result[5] == RaceResult{5, 10});
    REQUIRE(result[6] == RaceResult{6, 6});
    REQUIRE(result[7] == RaceResult{7, 0});
}

TEST_CASE("CountWinningRaces", "[day6]") {
    RaceRecord record{7, 9};
    auto       races = FindPossibleRaces(record);
    auto       wins  = CountWinningRaces(record, races);

    REQUIRE(4 == wins);
}

TEST_CASE("Solve Part 1", "[day6]") {
    std::vector<RaceRecord> exampleInput{{7, 9}, {15, 40}, {30, 200}};
    REQUIRE(288 == SolvePart1(exampleInput));
    std::vector<RaceRecord> puzzleInput{
        {40, 233}, {82, 1011}, {84, 1110}, {92, 1487}};
    REQUIRE(3316275 == SolvePart1(puzzleInput));
}

TEST_CASE("Solve Part 2", "[day6]") {
    REQUIRE(71503 == SolvePart2({71530, 940200}));
    REQUIRE(27'102'791 == SolvePart2({40828492, 233'101'111'101'487}));
}

std::vector<RaceResult> FindPossibleRaces(const RaceRecord& record) {
    std::vector<RaceResult> result;
    for (int holdTime = 0; holdTime <= record.time; ++holdTime) {
        result.emplace_back(holdTime, (record.time - holdTime) * holdTime);
    }
    return result;
}

int CountWinningRaces(
    const RaceRecord& record, const std::vector<RaceResult>& results) {
    // i'm not sure why I can't use a pipe operator outside a loop
    // tells me it can't find the operator
    // auto winningRange =
    // results | std::ranges::views::filter([&record](const auto& aResult) {
    // aResult.raceDistance > record.distance;
    //});
    // return std::ranges::size(winningRange);
    int result = 0;
    for (const auto& notNeeded :
         results | std::ranges::views::filter([&record](const auto& aResult) {
             return aResult.raceDistance > record.distance;
         })) {
        ++result;
    }
    return result;
}

int SolvePart1(std::vector<RaceRecord>& puzzleInput) {
    // inelegent. Doesn't use algorithms or ranges. but I just want an answer
    // right now
    int result = 1;
    for (const auto& input : puzzleInput) {
        result *= CountWinningRaces(input, FindPossibleRaces(input));
    }
    return result;
}

int64_t SolvePart2(const RaceRecord& input) {
    int64_t result = 0;
    // thanks to Sigmund for suggesting brute force
    double possibleSolutions = input.time;
    for (int possibleSpeed = 0; possibleSpeed < input.time; ++possibleSpeed) {
        auto distance = (input.time - possibleSpeed) * possibleSpeed;
        if (distance > input.distance) {
            ++result;
        }
        auto fraction = possibleSpeed / possibleSolutions;
        int  rounder  = fraction * 100000;
        if (rounder == 10000 || rounder == 200000 || rounder == 50000 ||
            rounder == 80000 || rounder == 90000) {
            std::cout << fraction << "\n";
        }
    }
    return result;
}