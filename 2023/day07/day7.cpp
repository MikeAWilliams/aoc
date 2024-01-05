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
    int classifierScore = -100; // I will know if I forget to set it

public:
    Hand(std::string cards, int bid);
    Hand(std::string cards, int bid, bool useJokers);
    Hand(std::string line);
    Hand(std::string line, bool useJokers);
    int         bid;
    std::string cards;
    bool        useJokers = false;

    void SetClassifierScore();
    void SetClassifierScoreWithJokers(std::unordered_map<char, int> cardCount);
    void
    SetClassifierScoreNoJokers(const std::unordered_map<char, int>& cardCount);
    std::unordered_map<char, int> CountCards();
    bool                          operator<(const Hand& right) const;
};

std::vector<Hand> GetPuzzleData(const std::vector<std::string>& lines);
std::vector<Hand> GetPuzzleDataPart2(const std::vector<std::string>& lines);
std::vector<std::string> GetPuzzleInput();

int64_t SolveEitherPart(std::vector<Hand> puzzleInput);

TEST_CASE("Less than operator", "[day7]") {
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
    // an example from a bug i found?

    REQUIRE(Hand{"29Q38", 10} < Hand{"9A8K5", 20});
}

TEST_CASE("Less than operator with jokers", "[day7]") {
    // jokers can make 5 of a kind
    REQUIRE(Hand{"AAAAK", 10, true} < Hand{"AAAAJ", 20, true});
    REQUIRE(Hand{"AAAAK", 10, true} < Hand{"AAAJJ", 20, true});
    REQUIRE(Hand{"AAAAK", 10, true} < Hand{"AAJJJ", 20, true});
    REQUIRE(Hand{"AAAAK", 10, true} < Hand{"AJJJJ", 20, true});
    REQUIRE(Hand{"AAAAK", 10, true} < Hand{"JJJJJ", 20, true});

    // jokers can make 4 of a kind
    REQUIRE(Hand{"AAAQQ", 10, true} < Hand{"AAAKJ", 20, true});
    REQUIRE(Hand{"AAAQQ", 10, true} < Hand{"JJJKQ", 20, true});
    REQUIRE(Hand{"AAAQQ", 10, true} < Hand{"KKKJQ", 20, true});
    REQUIRE(Hand{"AAAQQ", 10, true} < Hand{"KKJJQ", 20, true});
    REQUIRE(Hand{"AAAQQ", 10, true} < Hand{"KJJJQ", 20, true});

    // jokers can make a full house
    REQUIRE(Hand{"AAAKQ", 10, true} < Hand{"AAJQQ", 20, true});

    // jokers can make 3 of a kind
    REQUIRE(Hand{"AAKQQ", 10, true} < Hand{"AKJQQ", 20, true});

    // jokers can't make 2 pair

    // jokers can make one pair
    REQUIRE(Hand{"AKQT9", 10, true} < Hand{"KQATJ", 20, true});

    // same classifier but j is the lowest
    REQUIRE(Hand{"JQQQK", 10, true} < Hand{"2QQQQ", 20, true});
    REQUIRE(Hand{"QJQQK", 10, true} < Hand{"Q2QQQ", 20, true});
    REQUIRE(Hand{"QQJQK", 10, true} < Hand{"QQ2QQ", 20, true});
    REQUIRE(Hand{"QQQJK", 10, true} < Hand{"QQQ2Q", 20, true});
    // this case doesn't work because the J makes 5 of a kind
    // REQUIRE(Hand{"QQQQJ", 10, true} < Hand{"QQQQ2", 20, true});

    // the lowest value 5 of a kind
    REQUIRE(Hand{"JJJJJ", 10, true} < Hand{"22222", 20, true});
}

TEST_CASE("Part1 example", "[day7]") {
    REQUIRE(
        6440 == SolveEitherPart(std::vector<Hand>{
                    Hand{"32T3K", 765},
                    Hand{"T55J5", 684},
                    Hand{"KK677", 28},
                    Hand{"KTJJT", 220},
                    Hand{"QQQJA", 483},
                }));
}

TEST_CASE("Part2 example", "[day7]") {
    REQUIRE(
        5905 == SolveEitherPart(std::vector<Hand>{
                    Hand{"32T3K", 765, true},
                    Hand{"T55J5", 684, true},
                    Hand{"KK677", 28, true},
                    Hand{"KTJJT", 220, true},
                    Hand{"QQQJA", 483, true},
                }));
}

TEST_CASE("Part1 real", "[day7]") {
    REQUIRE(241344943 == SolveEitherPart(GetPuzzleData(GetPuzzleInput())));
}

TEST_CASE("Part2 real", "[day7]") {
    REQUIRE(243101568 == SolveEitherPart(GetPuzzleDataPart2(GetPuzzleInput())));
}

Hand::Hand(std::string c, int b) : Hand(c, b, false) {}
Hand::Hand(std::string c, int b, bool j) : cards{c}, bid{b}, useJokers{j} {
    SetClassifierScore();
}
Hand::Hand(std::string line) : Hand(line, false) {}
Hand::Hand(std::string line, bool j) : useJokers{j} {
    cards = line.substr(0, 5);
    bid   = std::stoi(line.substr(6));
    // std::cout << cards << " " << bid << "\n";
    SetClassifierScore();
}

void Hand::SetClassifierScoreWithJokers(
    std::unordered_map<char, int> cardCount) {
    int jokerCount = cardCount['J'];
    if (5 == jokerCount) {
        SetClassifierScoreNoJokers(cardCount);
        return;
    }
    cardCount.erase('J');
    std::vector<std::pair<char, int>> vCount(
        cardCount.begin(), cardCount.end());
    std::sort(vCount.begin(), vCount.end(), [](const auto& l, const auto& r) {
        return l.second > r.second;
    });
    cardCount[vCount[0].first] += jokerCount;
    SetClassifierScoreNoJokers(cardCount);
}

constexpr int FIVE_OF_A_KIND  = 7;
constexpr int FOUR_OF_A_KIND  = 6;
constexpr int FULL_HOUSE      = 5;
constexpr int THREE_OF_A_KIND = 4;
constexpr int TWO_PAIR        = 3;
constexpr int TWO_OF_A_KIND   = 2;
constexpr int HIGH_CARD       = 1;

void Hand::SetClassifierScoreNoJokers(
    const std::unordered_map<char, int>& cardCount) {
    if (1 == cardCount.size()) {
        classifierScore = FIVE_OF_A_KIND;
        return;
    }

    // no matching cards, high card
    if (5 == cardCount.size()) {
        classifierScore = HIGH_CARD;
        return;
    }

    // one pair
    if (4 == cardCount.size()) {
        classifierScore = TWO_OF_A_KIND;
        return;
    }

    // copy out to a vector just so its easier to get at them
    // if this is slow rework mayb here?
    std::vector<std::pair<char, int>> vCount(
        cardCount.begin(), cardCount.end());

    // full house or 4 of a kind
    if (2 == vCount.size()) {
        if (4 == vCount[0].second || 4 == vCount[1].second) {
            classifierScore = FOUR_OF_A_KIND;
            return;
        }
        classifierScore = FULL_HOUSE;
        return;
    }
    // 3 of a kind or 2 pair
    if (3 == vCount[0].second || 3 == vCount[1].second ||
        3 == vCount[2].second) {
        classifierScore = THREE_OF_A_KIND;
        return;
    }
    classifierScore = TWO_PAIR;
}
void Hand::SetClassifierScore() {
    auto cardCount = CountCards();
    if (!useJokers || !cardCount.contains('J')) {
        SetClassifierScoreNoJokers(cardCount);
    } else {
        SetClassifierScoreWithJokers(cardCount);
    }
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

    static std::unordered_map<char, int> cardValuesNormal{
        {'2', 1},  {'3', 2},  {'4', 3},  {'5', 4},  {'6', 5},
        {'7', 6},  {'8', 7},  {'8', 8},  {'9', 9},  {'T', 10},
        {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14},
    };

    static std::unordered_map<char, int> cardValuesJoker{
        {'2', 1}, {'3', 2},  {'4', 3},  {'5', 4},  {'6', 5},
        {'7', 6}, {'8', 7},  {'8', 8},  {'9', 9},  {'T', 10},
        {'J', 0}, {'Q', 12}, {'K', 13}, {'A', 14},
    };
    for (int i = 0; i < 5; ++i) {
        // std::cout << i << " " << this->cards[i] << " " << right.cards[i]
        //<< "\n";
        if (this->cards[i] != right.cards[i]) {
            if (this->useJokers) {
                return cardValuesJoker[this->cards[i]] <
                       cardValuesJoker[right.cards[i]];
            } else {
                return cardValuesNormal[this->cards[i]] <
                       cardValuesNormal[right.cards[i]];
            }
        }
    }
    return false;
}

int64_t SolveEitherPart(std::vector<Hand> puzzleInput) {
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

std::vector<Hand> GetPuzzleDataPart2(const std::vector<std::string>& lines) {
    std::vector<Hand> result;
    for (const auto& line : lines) {
        result.emplace_back(line, true);
    }
    return result;
}