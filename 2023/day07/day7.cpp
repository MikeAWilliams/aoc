// main.cpp
#include <catch2/catch_test_macros.hpp>
#include <string>

struct hand {
    std::string cards;
    int         bid;
    bool        operator<(const hand& right) const;
};

TEST_CASE("TestTheWrongLessThanImplementation", "[day7]") {
    REQUIRE(hand{"abcdef", 10} < hand{"abcdef", 20});
}

bool hand::operator<(const hand& right) const { return this->bid < right.bid; }