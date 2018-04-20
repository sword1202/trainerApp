#include <sstream>
#include "catch.hpp"
#include "VxFile.h"
#include "Primitives.h"

using namespace CppUtils;

TEST_CASE("VxFile parse") {
    std::string data = "lyricsStart* Yesterday(99,1800), all my troubles seem so far away.(2000,345) lyricsEnd* 24000 D5 99 1800 E3 2000 345 * 25";
    std::stringstream stream(data);
    VxFile vxFile(stream);

    const std::string &lyrics = vxFile.getLyrics();
    REQUIRE(lyrics == "Yesterday, all my troubles seem so far away.");
    REQUIRE(vxFile.getTicksPerMinute() == 24000);
    REQUIRE(vxFile.getDistanceInTicksBetweenLastPitchEndAndTrackEnd() == 25);
    REQUIRE(vxFile.getDurationInTicks() == 2000 + 345 + 25);
    REQUIRE(Primitives::CompareFloats(vxFile.getDurationInSeconds(), 60.0 / 24000 * (2000 + 345 + 25)));
}