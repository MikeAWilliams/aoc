// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>

struct MapRange {
    int destinationStart;
    int sourceStart;
    int length;

    MapRange(const std::vector<int>& ints) {
        if (3 != ints.size()) {
            throw "bad range data in";
        }
        destinationStart = ints[0];
        sourceStart      = ints[1];
        length           = ints[2];
    }
};

class ElfMap {
    std::unordered_map<int, int> map;

public:
    ElfMap() = default;
    int  Get(int source) const;
    void Add(int source, int destination);
    void Add(const MapRange& range);
};

class MapChain {
    std::vector<ElfMap> maps;

public:
    MapChain() = default;
    void Add(ElfMap toAdd);
    int  Get(int firstSource) const;
};

struct PuzzleData {
    std::vector<int> seedNumbers;
    MapChain         mapChain;
};

int                      Solve(const std::vector<std::string>& lines);
int                      SolvePart2(const std::vector<std::string>& lines);
PuzzleData               GetPuzzleData(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();

TEST_CASE("ElfMap", "[day5]") {
    ElfMap testObject;
    testObject.Add(1, 2);
    REQUIRE(2 == testObject.Get(1));
    REQUIRE(7 == testObject.Get(7));

    testObject.Add(MapRange({50, 98, 2}));
    REQUIRE(97 == testObject.Get(97));
    REQUIRE(50 == testObject.Get(98));
    REQUIRE(51 == testObject.Get(99));
    REQUIRE(100 == testObject.Get(100));
}

TEST_CASE("MapChain", "[day5]") {
    MapChain testObject;
    ElfMap   map1;
    ElfMap   map2;

    map1.Add(50, 100);
    map2.Add(100, 150);

    testObject.Add(map1);
    testObject.Add(map2);

    REQUIRE(150 == testObject.Get(50));
}

TEST_CASE("Part 1 easy", "[day5]") {
    REQUIRE(
        35 == Solve(std::vector<std::string>{
                  "seeds: 79 14 55 13",
                  "",
                  "seed-to-soil map:",
                  "50 98 2",
                  "52 50 48",
                  "",
                  "soil-to-fertilizer map:",
                  "0 15 37",
                  "37 52 2",
                  "39 0 15",
                  "",
                  "fertilizer-to-water map:",
                  "49 53 8",
                  "0 11 42",
                  "42 0 7",
                  "57 7 4",
                  "",
                  "water-to-light map:",
                  "88 18 7",
                  "18 25 70",
                  "",
                  "light-to-temperature map:",
                  "45 77 23",
                  "81 45 19",
                  "68 64 13",
                  "",
                  "temperature-to-humidity map:",
                  "0 69 1",
                  "1 0 69",
                  "",
                  "humidity-to-location map:",
                  "60 56 37",
                  "56 93 4"}));
}

TEST_CASE("Part 1 from file", "[day5]") {
    REQUIRE(35 == Solve(GetPuzzleInput()));
}

void ElfMap::Add(int source, int destination) {
    map.insert({source, destination});
}

void ElfMap::Add(const MapRange& range) {
    int source      = range.sourceStart;
    int destination = range.destinationStart;
    for (int i = 0; i < range.length; ++i) {
        this->Add(source, destination);
        source++;
        destination++;
    }
}

int ElfMap::Get(int source) const {
    if (map.contains(source)) {
        return map.at(source);
    }
    return source;
}

void MapChain::Add(ElfMap inMap) { maps.push_back(inMap); }

int MapChain::Get(int firstSource) const {
    int result = firstSource;
    for (const auto& map : maps) {
        result = map.Get(result);
    }
    return result;
}

PuzzleData GetPuzzleData(const std::vector<std::string>& lines) {
    PuzzleData       result;
    std::string_view seedsString(lines[0].data() + 7, lines[0].back());
    auto             seedsView = std::views::split(seedsString, ' ') |
                     std::ranges::views::filter([](const auto subRange) {
                         return std::string_view(subRange).size() > 0;
                     }) |
                     std::ranges::views::transform([](const auto subRange) {
                         return std::atoi(std::string_view(subRange).data());
                     });
    result.seedNumbers =
        std::vector(std::begin(seedsView), std::end(seedsView));

    ElfMap currentMap;
    for (int lineIndex = 3; lineIndex < lines.size(); ++lineIndex) {
        if (0 == lines[lineIndex].length()) {
            lineIndex += 2;
            result.mapChain.Add(currentMap);
            currentMap = ElfMap();
        }
        auto rangeView =
            std::views::split(lines[lineIndex], ' ') |
            std::ranges::views::filter([](const auto subRange) {
                return std::string_view(subRange).size() > 0;
            }) |
            std::ranges::views::transform([](const auto subRange) {
                return std::atoi(std::string_view(subRange).data());
            });
        std::vector<int> rangeVector(
            std::begin(rangeView), std::end(rangeView));
        currentMap.Add(rangeVector);
    }

    return result;
}
int Solve(const std::vector<std::string>& lines) {
    PuzzleData data   = GetPuzzleData(lines);
    int        result = std::numeric_limits<int>::max();
    for (auto seedNumber : data.seedNumbers) {
        result = std::min(result, data.mapChain.Get(seedNumber));
    }
    return result;
}

int SolvePart2(const std::vector<std::string>& lines) { return 0; }

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