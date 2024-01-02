// main.cpp
#include <catch2/catch_test_macros.hpp>
#include <compare>
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

std::vector<RaceResult> FindPossibleRaces(const RaceRecord& record) {
    std::vector<RaceResult> result;
    for (int holdTime = 0; holdTime <= record.time; ++holdTime) {
        result.emplace_back(holdTime, (record.time - holdTime) * holdTime);
    }
    return result;
}