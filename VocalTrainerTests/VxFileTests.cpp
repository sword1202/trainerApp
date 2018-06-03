#include "catch.hpp"
#include "VxPitch.h"

TEST_CASE("VxPitch intersection test") {
    VxPitch a;
    a.startTickNumber = 0;
    a.ticksCount = 100;
    VxPitch b;
    b.startTickNumber = 100;
    b.ticksCount = 200;

    REQUIRE(!a.intersectsWith(b));
    REQUIRE(!b.intersectsWith(a));

    a.startTickNumber = 0;
    a.ticksCount = 100;
    b.startTickNumber = 99;
    b.ticksCount = 200;

    REQUIRE(a.intersectsWith(b));
    REQUIRE(b.intersectsWith(a));

    a.startTickNumber = 0;
    a.ticksCount = 100;
    b.startTickNumber = 0;
    b.ticksCount = 100;

    REQUIRE(a.intersectsWith(b));
    REQUIRE(b.intersectsWith(a));


    a.startTickNumber = 0;
    a.ticksCount = 100;
    b.startTickNumber = 50;
    b.ticksCount = 20;
    REQUIRE(a.intersectsWith(b));
    REQUIRE(b.intersectsWith(a));
}

TEST_CASE("iteratePitchesInTickRange test") {

}