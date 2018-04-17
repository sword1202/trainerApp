#include <sstream>
#include "catch.hpp"
#include "VxFile.h"

TEST_CASE("VxFile parse") {
    std::string data = "lyricsStart* Yesterday(99,1800), all my troubles seem so far away.(2000,345) lyricsEnd* 24000 D5 99 1800 E3 2000 345 * 0";
    std::stringstream stream(data);
    VxFile vxFile(stream);
}