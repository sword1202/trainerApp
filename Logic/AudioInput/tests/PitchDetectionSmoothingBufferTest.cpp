//
// Created by Semyon Tikhonenko on 1/25/17.
//
#include "../CppUtils/tests/catch.hpp"
#include "Logic/PitchDetectionSmoothingAudioBuffer.h"

TEST_CASE("pitch detection smoothing buffer test") {
    PitchDetectionSmoothingAudioBuffer buffer(4, 5);
    REQUIRE(buffer.getCurrentBufferSize() == 0);
    std::vector<int16_t > piece1 = {1, 2, 3, 4};
    std::vector<int16_t > piece2 = {9, 9, 3, 9, 9};
    REQUIRE(buffer.getRunPitchDetectionBufferIfReady(piece1.data(), 4) == 0);
    REQUIRE(buffer.getRunPitchDetectionBufferIfReady(piece2.data(), 5) == 0);
    REQUIRE(buffer.getRunPitchDetectionBufferIfReady(piece1.data(), 4) == 0);
    const int16_t *bufferForPitch = buffer.getRunPitchDetectionBufferIfReady(piece1.data(), 4);
    REQUIRE(buffer.getCurrentBufferSize() == 17);
    REQUIRE(bufferForPitch != 0);
    REQUIRE(std::equal(bufferForPitch, bufferForPitch + 17,
                       std::vector<int16_t >{1, 2, 3, 4,
                                             9, 9, 3, 9, 9,
                                             1, 2, 3, 4,
                                             1, 2, 3, 4}.begin()));
    bufferForPitch = buffer.getRunPitchDetectionBufferIfReady(piece1.data(), 4);
    REQUIRE(buffer.getCurrentBufferSize() == 17);
    REQUIRE(std::equal(bufferForPitch, bufferForPitch + 17,
                       std::vector<int16_t >{9, 9, 3, 9, 9,
                                             1, 2, 3, 4,
                                             1, 2, 3, 4,
                                             1, 2, 3, 4}.begin()));

    //smoothLevel = 1
    buffer = PitchDetectionSmoothingAudioBuffer(1, 5);
    bufferForPitch = buffer.getRunPitchDetectionBufferIfReady(piece1.data(), 4);
    REQUIRE(buffer.getCurrentBufferSize() == 4);
    REQUIRE(std::equal(bufferForPitch, bufferForPitch + 4,
                       std::vector<int16_t >{1, 2, 3, 4}.begin()));

}

