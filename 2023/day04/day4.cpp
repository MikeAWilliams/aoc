// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>

struct CardData {
    int              number;
    std::vector<int> winningNumbers;
    std::vector<int> numbers;
    int              numberOfCopies = 1;

    CardData(const std::string&);
    int CountWinningNumbers();
    int ScorePart1();
};

int                      Solve(const std::vector<std::string>& lines);
int                      SolvePart2(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();

TEST_CASE("GameData", "[day4]") {
    CardData testObject("Card   1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53");
    REQUIRE(1 == testObject.number);
    REQUIRE(5 == testObject.winningNumbers.size());
    REQUIRE(8 == testObject.numbers.size());
    REQUIRE(8 == testObject.ScorePart1());

    testObject =
        CardData("Card 213:  8 24 48  5 27 13 71 26 17 21 | 63 11 89 35 69 98 "
                 "91 60 51 70 12 62 39 43 38 19 55 20 57 32  6 79 95 76 73");
    REQUIRE(213 == testObject.number);
    REQUIRE(10 == testObject.winningNumbers.size());
    REQUIRE(25 == testObject.numbers.size());
    REQUIRE(0 == testObject.ScorePart1());
}

TEST_CASE("solve part 1 easy", "[day4]") {
    REQUIRE(
        13 == Solve(std::vector<std::string>{
                  "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53",
                  "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19",
                  "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1",
                  "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83",
                  "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36",
                  "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"}));
}

TEST_CASE("solve part 1 from file", "[day4]") {
    REQUIRE(25183 == Solve(GetPuzzleInput()));
}

TEST_CASE("solve part 2 easy", "[day4]") {
    REQUIRE(
        30 == SolvePart2(std::vector<std::string>{
                       "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53",
                       "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19",
                       "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1",
                       "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83",
                       "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36",
                       "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"}));
}

TEST_CASE("solve part 2 from file", "[day4]") {
    REQUIRE(5667240 == SolvePart2(GetPuzzleInput()));
}

int Solve(const std::vector<std::string>& lines) {
    // with no ranges::to or fold functions I a temp vector
    std::vector<int> scores;
    scores.reserve(lines.size());
    std::ranges::transform(
        lines, std::back_insert_iterator(scores),
        [](const auto& line) { return CardData(line).ScorePart1(); });
    return std::accumulate(std::cbegin(scores), std::cend(scores), 0);
}

int SolvePart2(const std::vector<std::string>& lines) {
    std::vector<CardData> cards;
    cards.reserve(lines.size());
    std::ranges::transform(
        lines, std::back_insert_iterator(cards),
        [](const auto& line) { return CardData(line); });

    for (size_t i = 0; i < cards.size(); ++i) {
        auto winsOnCard = cards[i].CountWinningNumbers();
        for (size_t j = i + 1; j <= i + winsOnCard; ++j) {
            if (j >= cards.size()) {
                throw "index out of bouds";
            }
            cards[j].numberOfCopies += cards[i].numberOfCopies;
        }
    }

    return std::accumulate(
        std::cbegin(cards), std::cend(cards), 0,
        [](int valueSoFar, const auto& aCard) {
            return valueSoFar + aCard.numberOfCopies;
        });
}

CardData::CardData(const std::string& line) {
    number          = std::atoi(line.data() + 5);
    auto colonIndex = line.find(':');
    if (std::string::npos == colonIndex) {
        throw "can't find a colon in a line";
    }
    auto pipeIndex = line.find('|', colonIndex);
    if (std::string::npos == pipeIndex) {
        throw "can't find a pipe in a line";
    }
    std::string_view winningString{
        line.begin() + colonIndex + 2, line.begin() + pipeIndex - 1};
    // std::cout << colonIndex << " " << pipeIndex << " winning numbers " <<
    // winningString << "\n";
    auto winningView = std::views::split(winningString, ' ') |
                       std::ranges::views::filter([](const auto subRange) {
                           return std::string_view(subRange).size() > 0;
                       }) |
                       std::ranges::views::transform([](const auto ptr) {
                           return std::atoi(std::string_view(ptr).data());
                       });
    // would have loved to use std::ranges::to<std::vector> above but its not
    // implemented yet
    this->winningNumbers =
        std::vector(std::begin(winningView), std::end(winningView));

    std::string_view numbersString{line.begin() + pipeIndex + 2, line.end()};
    auto             numbersView = std::views::split(numbersString, ' ') |
                       std::ranges::views::filter([](const auto subRange) {
                           return std::string_view(subRange).size() > 0;
                       }) |
                       std::ranges::views::transform([](const auto subRange) {
                           return std::atoi(std::string_view(subRange).data());
                       });
    this->numbers = std::vector(std::begin(numbersView), std::end(numbersView));
}

int CardData::CountWinningNumbers() {
    return std::ranges::count_if(this->numbers, [this](auto num) {
        return std::ranges::find(this->winningNumbers, num) !=
               this->winningNumbers.end();
    });
}

int CardData::ScorePart1() {
    return static_cast<int>(std::pow(2.0, this->CountWinningNumbers() - 1));
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