// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <compare>
#include <cstdlib>
#include <limits>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

struct RaceRecord {
    int time;
    int distance;
};

struct RaceResult {
    int  buttonTime;
    int  raceDistance;
    auto operator<=>(const RaceResult&) const = default;
};

std::vector<RaceResult> FindPossibleRaces(const RaceRecord& record);
int                     CountWinningRaces(
                        const RaceRecord& record, const std::vector<RaceResult>& results);
int SolvePart1(std::vector<RaceRecord>& puzzleInput);

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