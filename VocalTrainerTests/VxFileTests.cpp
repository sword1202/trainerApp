#include <sstream>
#include <iostream>
#include "catch.hpp"
#include "VxFile.h"
#include "Primitives.h"

using namespace CppUtils;

TEST_CASE("VxFile parse") {
    std::string data = "lyricsStart* Yesterday, all my troubles<10,2900> seem so far away.<3000,5925> lyricsEnd* 24000 D5 99 1800 E3 2000 345 * 25";
    std::stringstream stream(data);
    VxFile vxFile(stream);

    const std::vector<VxLyricsInterval> &lyrics = vxFile.getLyrics();
    REQUIRE(lyrics.size() == 2);
    REQUIRE(lyrics[0].startTimestampInMilliseconds == 10);
    REQUIRE(lyrics[0].endTimestampInMilliseconds == 2900);
    REQUIRE(lyrics[0].lyrics == L"Yesterday, all my troubles");
    REQUIRE(lyrics[1].startTimestampInMilliseconds == 3000);
    REQUIRE(lyrics[1].endTimestampInMilliseconds == 5925);
    REQUIRE(lyrics[1].lyrics == L" seem so far away.");
    REQUIRE(vxFile.getTicksPerMinute() == 24000);
    REQUIRE(vxFile.getDistanceInTicksBetweenLastPitchEndAndTrackEnd() == 25);
    REQUIRE(vxFile.getDurationInTicks() == 2000 + 345 + 25);
    REQUIRE(Primitives::CompareFloats(vxFile.getDurationInSeconds(), 60.0 / 24000 * (2000 + 345 + 25)));
}

namespace boost {
    void assertion_failed(char const * expr, char const * function, char const * file, long line) {
        throw std::runtime_error("Assertion Failed");
    }
}

TEST_CASE("VxFile parse validatePitches failed") {
    std::string data = "noLyrics* 250 D2 99 0";
    std::stringstream stream(data);
    REQUIRE_THROWS_AS(VxFile(stream), std::runtime_error);
}

TEST_CASE("VxFile parse validatePitches successed") {
    std::string data = "noLyrics* 250 D2 120 50 E3 99 40 * 76";
    std::stringstream stream(data);
    VxFile vxFile(stream);
}