#include "catch.hpp"
#include "PitchRenderer.h"
#include "VxFileAudioDataGenerator.h"

class PitchRenderer1 : public PitchRenderer {
public:
    PitchRenderer1(int sampleRate) : PitchRenderer(sampleRate) {
    }

private:
    void render(const Pitch &pitch, short *buffer, int length) override {
        std::fill(buffer, buffer + length, pitch.getPerfectFrequencyIndex());
    }
};


/*
 * ====================10============================
 *     ======20======        ^           ===========30==========
 *                                              =====50======
 * */

TEST_CASE("VxFileGenerator test 1") {
    std::vector<VxPitch> pitches;
    VxPitch vxPitch;
    vxPitch.pitch = Pitch::fromPerfectFrequencyIndex(10);
    vxPitch.startTickNumber = 0;
    vxPitch.ticksCount = 50;
    pitches.push_back(vxPitch);
    vxPitch.pitch = Pitch::fromPerfectFrequencyIndex(20);
    vxPitch.startTickNumber = 20;
    vxPitch.ticksCount = 10;
    pitches.push_back(vxPitch);
    vxPitch.pitch = Pitch::fromPerfectFrequencyIndex(30);
    vxPitch.startTickNumber = 40;
    vxPitch.ticksCount = 30;
    pitches.push_back(vxPitch);
    vxPitch.pitch = Pitch::fromPerfectFrequencyIndex(50);
    vxPitch.startTickNumber = 45;
    vxPitch.ticksCount = 15;
    pitches.push_back(vxPitch);

    int ticksPerSecond = 10;
    VxFile vxFile(pitches, 0, ticksPerSecond);
    VxFileAudioDataGeneratorConfig config;
    config.sampleRate = 10;
    config.outBufferSize = 10;
    VxFileAudioDataGenerator generator(new PitchRenderer1(config.sampleRate), vxFile, config);
    generator.setSeek(35);

    std::vector<short> buffer(10);

    // no data rendered, should return silence
    generator.readNextSamplesBatch(buffer.data());
    REQUIRE(buffer == std::vector<short>(10, 0));

    while (generator.renderNextPitchIfPossible());

    std::vector<short> expected;

    generator.readNextSamplesBatch(buffer.data());
    std::fill_n(std::back_inserter(expected), 5, 10);
    std::fill_n(std::back_inserter(expected), 5, 20);
    REQUIRE(buffer == expected);
    expected.clear();

    generator.readNextSamplesBatch(buffer.data());
    std::fill_n(std::back_inserter(expected), 5, 30);
    std::fill_n(std::back_inserter(expected), 5, 40);
    REQUIRE(buffer == expected);
    expected.clear();

    generator.readNextSamplesBatch(buffer.data());
    std::fill_n(std::back_inserter(expected), 5, 40);
    std::fill_n(std::back_inserter(expected), 5, 30);
    REQUIRE(buffer == expected);
    expected.clear();

    std::fill(buffer.begin(), buffer.end(), 0);
    REQUIRE(generator.readNextSamplesBatch(buffer.data()) == 5);
    std::fill_n(std::back_inserter(expected), 5, 30);
    std::fill_n(std::back_inserter(expected), 5, 0);
    REQUIRE(buffer == expected);
}

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
}

TEST_CASE("VxFileGenerator test 2") {
    VxFile vxFile(std::vector<VxPitch> {
        {Pitch::fromPerfectFrequencyIndex(1), 0, 100}, {Pitch::fromPerfectFrequencyIndex(2), 100, 100},
            {Pitch::fromPerfectFrequencyIndex(3), 200, 100}},
            0, 100);
    VxFileAudioDataGeneratorConfig config;
    config.sampleRate = 100;
    config.outBufferSize = 100;
    VxFileAudioDataGenerator generator(new PitchRenderer1(config.sampleRate), vxFile, config);

    std::vector<short> buffer(100);

    REQUIRE(generator.renderNextPitchIfPossible());
    generator.readNextSamplesBatch(buffer.data());
    REQUIRE(buffer == std::vector<short>(100, 1));

    REQUIRE(generator.renderNextPitchIfPossible());
    generator.readNextSamplesBatch(buffer.data());
    REQUIRE(buffer == std::vector<short>(100, 2));

    REQUIRE(generator.renderNextPitchIfPossible());
    generator.readNextSamplesBatch(buffer.data());
    REQUIRE(buffer == std::vector<short>(100, 3));

    REQUIRE(!generator.renderNextPitchIfPossible());
}