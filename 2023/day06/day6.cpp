// main.cpp
#include <catch2/catch_test_macros.hpp>
#include <vector>

struct RaceRecord {
    int time;
    int distance;
};

struct RaceResult {
    int buttonTime;
    int raceDistance;
};

std::vector<RaceResult> FindPossibleRaces(const RaceRecord& record);

TEST_CASE("FindPossibleRaces", "[day6]") {
    auto result = FindPossibleRaces({7, 9});
    REQUIRE(8 == result.size());
}

std::vector<RaceResult> FindPossibleRaces(const RaceRecord& record) {
    return std::vector<RaceResult>{};
}