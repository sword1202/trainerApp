#include "catch.hpp"
#include "VxFile.h"
#include "Primitives.h"

using namespace CppUtils;

TEST_CASE("VxFile parser test 1") {
    std::string str = "VX 1 123 C5 16 P 16 E#4 1 F4 -8";
    VxFile file;
    file.parse(str);
    int beatsPerMinute;
    auto vocalPart = file.getVocalPart()
    REQUIRE(beatsPerMinute == 123);
    REQUIRE(vocalPart.getNotes().size() == 3);

    double beatsCount = 5.25;
    REQUIRE(Primitives::CompareFloatsUsingEpsilon(vocalPart.getDurationInSeconds(), beatsCount / beatsPerMinute * 60.0, 0.0001));
    REQUIRE(vocalPart.getDurationInTicks() == 21);

    REQUIRE(vocalPart.getNotes().at(0).pitch.getPerfectFrequencyIndex() == Pitch("C5").getPerfectFrequencyIndex());
    REQUIRE(vocalPart.getNotes().at(0).startTickNumber == 0);
    REQUIRE(vocalPart.getNotes().at(0).ticksCount == 1);

    REQUIRE(vocalPart.getNotes().at(1).pitch.getPerfectFrequencyIndex() == Pitch("E#4").getPerfectFrequencyIndex());
    REQUIRE(vocalPart.getNotes().at(1).startTickNumber == 2);
    REQUIRE(vocalPart.getNotes().at(1).ticksCount == 16);

    REQUIRE(vocalPart.getNotes().at(2).pitch.getPerfectFrequencyIndex() == Pitch("F4").getPerfectFrequencyIndex());
    REQUIRE(vocalPart.getNotes().at(2).startTickNumber == 18);
    REQUIRE(vocalPart.getNotes().at(2).ticksCount == 3);
}