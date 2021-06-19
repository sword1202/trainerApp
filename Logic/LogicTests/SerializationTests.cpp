#include "catch.hpp"
#include <sstream>
#include "BinaryArchive.h"

using namespace CppUtils;

TEST_CASE("empty vector<short> read write test") {
    std::stringstream str;
    std::vector<short> v;
    Serialization::WriteObjectToBinaryStream(v, str);
    std::vector<short> v2;
    Serialization::ReadObjectFromBinaryStream(v2, str);
    REQUIRE(v2.empty());
}