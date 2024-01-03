// main.cpp
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Hand {
private:
    int classifierScore =
        100; // set this high so I will know if I forget to set it

public:
    Hand(std::string cards, int bid);
    Hand(std::string line);
    int         bid;
    std::string cards;

    void                          SetClassifierScore();
    std::unordered_map<char, int> CountCards();
    bool                          operator<(const Hand& right) const;
};

std::vector<Hand>        GetPuzzleData(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();

int64_t SolvePart1(std::vector<Hand> puzzleInput);

TEST_CASE("Less than operator", "[day7]") {
    /*
    // decide by hand classification
    REQUIRE(Hand{"AAAAK", 10} < Hand{"AAAAA", 20});
    REQUIRE(Hand{"AAAQQ", 10} < Hand{"AAAAK", 20});
    REQUIRE(Hand{"AAAKQ", 10} < Hand{"AAAKK", 20});
    REQUIRE(Hand{"AAKKQ", 10} < Hand{"AAAKQ", 20});
    REQUIRE(Hand{"AAJKQ", 10} < Hand{"AAKKQ", 20});
    REQUIRE(Hand{"AKQJ9", 10} < Hand{"AAKQJ", 20});

    // decide by first card
    REQUIRE(Hand{"KAAAA", 10} < Hand{"AAAAK", 20});
    REQUIRE(Hand{"AKAAA", 10} < Hand{"AAAAK", 20});
    REQUIRE(Hand{"AAKAA", 10} < Hand{"AAAAK", 20});
    REQUIRE(Hand{"AAAKA", 10} < Hand{"AAAAK", 20});

    // include a T because its weird
    REQUIRE(Hand{"TAAAA", 10} < Hand{"JAAAA", 20});

    // they are equal
    REQUIRE_FALSE(Hand{"AAAAK", 10} < Hand{"AAAAK", 20});
    */
    // an example from a bug i found?

    REQUIRE(Hand{"29Q38", 10} < Hand{"9A8K5", 20});
}

TEST_CASE("Part1 example", "[day7]") {
    SKIP();
    REQUIRE(
        6440 == SolvePart1(std::vector<Hand>{
                    Hand{"32T3K", 765},
                    Hand{"T55J5", 684},
                    Hand{"KK677", 28},
                    Hand{"KTJJT", 220},
                    Hand{"QQQJA", 483},
                }));
}

TEST_CASE("Part1 real", "[day7]") {
    REQUIRE(241344943 == SolvePart1(GetPuzzleData(GetPuzzleInput())));
}

Hand::Hand(std::string c, int b) : cards{c}, bid{b} { SetClassifierScore(); }
Hand::Hand(std::string line) {
    cards = line.substr(0, 5);
    bid   = std::stoi(line.substr(6));
    // std::cout << cards << " " << bid << "\n";
    SetClassifierScore();
}

void Hand::SetClassifierScore() {
    auto cardCount = CountCards();

    // 5 of a kind
    if (1 == cardCount.size()) {
        classifierScore = 7;
        return;
    }

    // no matching cards, high card
    if (5 == cardCount.size()) {
        classifierScore = 1;
        return;
    }

    // one pair
    if (4 == cardCount.size()) {
        classifierScore = 2;
        return;
    }

    // copy out to a vector just so its easier to get at them
    // if this is slow rework mayb here?
    std::vector<std::pair<char, int>> vCount(
        cardCount.begin(), cardCount.end());

    // full house or 4 of a kind
    if (2 == vCount.size()) {
        if (4 == vCount[0].second || 4 == vCount[1].second) {
            classifierScore = 6;
            return;
        }
        classifierScore = 5;
        return;
    }
    // 3 of a kind or 2 pair
    if (3 == vCount[0].second || 3 == vCount[1].second ||
        3 == vCount[2].second) {
        classifierScore = 4;
        return;
    }
    classifierScore = 3;
}

std::unordered_map<char, int> Hand::CountCards() {
    std::unordered_map<char, int> result;
    for (auto card : cards) {
        if (!result.contains(card)) {
            result.insert({card, 1});
        } else {
            result[card]++;
        }
    }
    return result;
}

bool Hand::operator<(const Hand& right) const {
    // std::cout << "comparing this " << this->cards << " "
    //<< this->classifierScore << " with " << right.cards << " "
    //<< right.classifierScore << "\n";
    if (this->classifierScore != right.classifierScore) {
        return this->classifierScore < right.classifierScore;
    }

    static std::unordered_map<char, int> cardValues{
        {'2', 1},  {'3', 2},  {'4', 3},  {'5', 4},  {'6', 5},
        {'7', 6},  {'8', 7},  {'8', 8},  {'9', 9},  {'T', 10},
        {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14},
    };
    for (int i = 0; i < 5; ++i) {
        // std::cout << i << " " << this->cards[i] << " " << right.cards[i]
        //<< "\n";
        if (this->cards[i] != right.cards[i]) {
            return cardValues[this->cards[i]] < cardValues[right.cards[i]];
        }
    }
    return false;
}

int64_t SolvePart1(std::vector<Hand> puzzleInput) {
    std::sort(puzzleInput.begin(), puzzleInput.end());

    int64_t result = 0;
    for (int i = 0; i < puzzleInput.size(); i++) {
        result += (i + 1) * puzzleInput[i].bid;
        // std::cout << (i + 1) << " " << puzzleInput[i].cards << " "
        //<< puzzleInput[i].bid << "\n";
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

std::vector<Hand> GetPuzzleData(const std::vector<std::string>& lines) {
    return std::vector<Hand>(lines.begin(), lines.end());
}