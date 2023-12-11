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
    int64_t destinationStart;
    int64_t sourceStart;
    int64_t length;
    MapRange(const std::vector<int64_t>& ints);
    bool    SourceInRange(int64_t source) const;
    int64_t GetValue(int64_t source) const;
};

struct ElfMap {
    std::vector<MapRange> ranges;

public:
    ElfMap() = default;
    int64_t Get(int64_t source) const;
    void    Add(const MapRange& range);
};

struct MapChain {
    std::vector<ElfMap> maps;

public:
    MapChain() = default;
    void    Add(ElfMap toAdd);
    int64_t Get(int64_t firstSource) const;
};

struct PuzzleData {
    std::vector<int64_t> seedNumbers;
    std::vector<std::pair<int64_t, int64_t>> seedNumberRanges;
    MapChain             mapChain;
};

int64_t                  Solve(const std::vector<std::string>& lines);
int64_t                  SolvePart2(const std::vector<std::string>& lines);
PuzzleData               GetPuzzleData(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();

TEST_CASE("ElfMap", "[day5]") {
    ElfMap testObject;
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

    map1.Add(MapRange{{100, 50, 1}});
    map2.Add(MapRange{{150, 100, 1}});

    testObject.Add(map1);
    testObject.Add(map2);

    REQUIRE(150 == testObject.Get(50));
}

TEST_CASE("MapRange", "[day5]") {
    MapRange testObject{{10, 5, 2}};
    REQUIRE(testObject.SourceInRange(5));
    REQUIRE(testObject.SourceInRange(6));
    REQUIRE_FALSE(testObject.SourceInRange(4));
    REQUIRE_FALSE(testObject.SourceInRange(7));

    REQUIRE(10 == testObject.GetValue(5));
    REQUIRE(11 == testObject.GetValue(6));
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
    REQUIRE(178159714 == Solve(GetPuzzleInput()));
}

TEST_CASE("Part 2 easy", "[day5]") {
    REQUIRE(
        46 == SolvePart2(std::vector<std::string>{
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

TEST_CASE("Part 2 from file", "[day5]") {
    // may not finish for the heat death of the universe
    REQUIRE(178159714 == SolvePart2(GetPuzzleInput()));
}

void ElfMap::Add(const MapRange& range) { this->ranges.push_back(range); }

int64_t ElfMap::Get(int64_t source) const {
    for (const auto& mapRange :
         this->ranges |
             std::ranges::views::filter([source](const auto& mapRange) {
                 return mapRange.SourceInRange(source);
             })) {
        return mapRange.GetValue(source);
    }
    return source;
}

void MapChain::Add(ElfMap inMap) { maps.push_back(inMap); }

int64_t MapChain::Get(int64_t firstSource) const {
    int64_t result = firstSource;
    for (const auto& map : maps) {
        result = map.Get(result);
    }
    return result;
}

MapRange::MapRange(const std::vector<int64_t>& ints) {
    if (3 != ints.size()) {
        throw "bad range data in";
    }
    destinationStart = ints[0];
    sourceStart      = ints[1];
    length           = ints[2];
}

bool MapRange::SourceInRange(int64_t source) const {
    return source >= this->sourceStart &&
           source < this->sourceStart + this->length;
}
int64_t MapRange::GetValue(int64_t source) const {
    int64_t result = (source - this->sourceStart) + this->destinationStart;
    if (result < 0) {
        std::cout << destinationStart << " " << sourceStart << " " << length
                  << " " << source << " " << result;
        throw "error got a negative";
    }
    return result;
}

PuzzleData GetPuzzleData(const std::vector<std::string>& lines) {
    PuzzleData       result;
    std::string_view seedsString(lines[0].data() + 7, lines[0].size() - 7);
    auto             seedsView = std::views::split(seedsString, ' ') |
                     std::ranges::views::filter([](const auto subRange) {
                         return std::string_view(subRange).size() > 0;
                     }) |
                     std::ranges::views::transform([](const auto subRange) {
                         return std::atoll(std::string_view(subRange).data());
                     });
    result.seedNumbers =
        std::vector<int64_t>(std::begin(seedsView), std::end(seedsView));

    // if I had chunk_view in 23...
    //auto pair_view = result.seedNumbers | std::ranges::chunk_view(2);
    //result.seedNumberRanges = std::vector<std::pair<int64_t, int64_t>>(std::begin(pair_view), std::end(pair_view));
    for(int i = 0; i < result.seedNumbers.size(); i += 2)
    {
        result.seedNumberRanges.push_back(std::pair(result.seedNumbers[i], result.seedNumbers[i+1]));
    }

    ElfMap currentMap;
    for (int64_t lineIndex = 3; lineIndex < lines.size(); ++lineIndex) {
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
                return std::atoll(std::string_view(subRange).data());
            });
        std::vector<int64_t> rangeVector(
            std::begin(rangeView), std::end(rangeView));
        currentMap.Add(rangeVector);
    }

    result.mapChain.Add(currentMap);
    return result;
}

void DumpPuzzleDataToFileForDebug(
    const PuzzleData& data, const std::string& fileName) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw "I can't open the output file";
    }
    file << "seeds:";
    for (auto seed : data.seedNumbers) {
        file << " " << seed;
    }
    file << "\n\n";

    for (const auto& map : data.mapChain.maps) {
        file << "thing-to-thing map:\n";
        for (const auto& mapRange : map.ranges) {
            file << mapRange.destinationStart << " " << mapRange.sourceStart
                 << " " << mapRange.length << "\n";
        }
        file << "\n";
    }
    file.close();
}

int64_t Solve(const std::vector<std::string>& lines) {
    PuzzleData data = GetPuzzleData(lines);
    //DumpPuzzleDataToFileForDebug(data, "../puzzle_out.txt");
    int64_t result = std::numeric_limits<int64_t>::max();
    for (auto seedNumber : data.seedNumbers) {
        result = std::min(result, data.mapChain.Get(seedNumber));
    }
    return result;
}

int64_t SolvePart2(const std::vector<std::string>& lines) { 
    PuzzleData data = GetPuzzleData(lines);
    int64_t result = std::numeric_limits<int64_t>::max();
    for (auto seedPair : data.seedNumberRanges) {
        for(int seedNumber = seedPair.first; seedNumber < seedPair.first + seedPair.second; ++seedNumber ){
            result = std::min(result, data.mapChain.Get(seedNumber));
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