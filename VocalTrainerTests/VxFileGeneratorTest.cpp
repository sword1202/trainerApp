#include "catch.hpp"
#include "PitchRenderer.h"
#include "VxFileAudioDataGenerator.h"
#include <sstream>

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

TEST_CASE("VxFileGenerator test 2") {
    VxFile vxFile(std::vector<VxPitch> {
        {Pitch::fromPerfectFrequencyIndex(1), 0, 100},
                    {Pitch::fromPerfectFrequencyIndex(2), 100, 100},
                    {Pitch::fromPerfectFrequencyIndex(3), 200, 100},
                    {Pitch::fromPerfectFrequencyIndex(4), 400, 100}},
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

    REQUIRE(generator.renderNextPitchIfPossible());
    generator.readNextSamplesBatch(buffer.data());
    REQUIRE(buffer == std::vector<short>(100, 0));

    generator.readNextSamplesBatch(buffer.data());
    REQUIRE(buffer == std::vector<short>(100, 4));

    REQUIRE(!generator.renderNextPitchIfPossible());
}

TEST_CASE("VxFileGenerator test 3") {
    auto data = "22 serialization::archive 16 0 0 400 0 0 228 0 0 0 50 14399 180 52 14579 180 53 14759 180 57 14939 180 55 15359 180 52 15539 180 48 15719 120 55 16229 90 53 16319 180 50 16499 180 46 16679 120 53 17219 60 52 17279 120 53 17399 60 52 17459 120 45 17579 60 52 17639 120 50 18239 180 52 18419 180 53 18599 180 57 18779 180 60 19199 240 58 19439 60 57 19499 60 55 19559 120 55 20069 90 58 20159 240 55 20399 120 57 20519 120 58 20639 240 57 20879 120 55 20999 120 57 21119 300 58 22079 420 57 22499 60 55 22559 240 58 22799 240 57 23039 240 58 23999 420 57 24419 60 55 24479 240 58 24719 240 57 24959 780 57 25919 360 55 26279 120 53 26399 240 55 26639 120 57 26759 120 57 26879 360 57 27839 360 55 28199 120 53 28319 240 55 28559 120 57 28679 120 57 28799 480 58 29879 240 57 30119 120 55 30239 240 53 30479 120 55 30599 120 58 30839 240 57 31079 120 55 31199 240 57 31439 120 58 31559 120 57 31679 540 57 32639 360 55 32999 120 53 33119 240 55 33359 120 57 33479 120 57 33599 360 57 34559 360 55 34919 120 53 35039 240 55 35279 120 57 35399 120 57 35519 480 58 36599 240 57 36839 120 55 36959 240 53 37199 120 55 37319 120 58 37439 240 58 37679 120 57 37799 120 55 37919 240 57 38159 120 58 38279 120 57 38399 540 50 39359 180 52 39539 180 53 39719 180 57 39899 180 55 40319 180 52 40499 180 48 40679 120 55 41219 60 53 41279 180 50 41459 180 46 41639 120 53 42179 60 52 42239 120 53 42359 60 52 42419 120 45 42539 60 52 42599 120 50 43199 180 52 43379 180 53 43559 180 57 43739 180 57 44099 60 60 44159 240 58 44399 60 57 44459 60 55 44519 120 55 45059 60 58 45119 240 55 45359 120 57 45479 120 58 45599 240 57 45839 120 55 45959 120 57 46079 300 58 47039 420 57 47459 60 55 47519 240 58 47759 240 57 47999 240 58 48959 420 57 49379 60 55 49439 240 58 49679 240 57 49919 780 57 50883 360 55 51243 120 53 51363 240 55 51603 120 57 51723 120 57 51843 360 57 52803 360 55 53163 120 53 53283 240 55 53523 120 57 53643 120 57 53763 480 58 54843 240 57 55083 120 55 55203 240 53 55443 120 55 55563 120 58 55803 240 57 56043 120 55 56163 240 57 56403 120 58 56523 120 57 56643 540 57 57603 360 55 57963 120 53 58083 240 55 58323 120 57 58443 120 57 58563 360 57 59523 360 55 59883 120 53 60003 240 55 60243 120 57 60363 120 57 60483 480 58 61563 240 57 61803 120 55 61923 240 53 62163 120 55 62283 120 58 62403 240 58 62643 120 57 62763 120 55 62883 240 57 63123 120 58 63243 120 57 63363 540 57 99839 360 55 100199 120 53 100319 240 55 100559 120 57 100679 120 57 100799 360 57 101759 360 55 102119 120 53 102239 240 55 102479 120 57 102599 120 57 102719 480 58 103799 240 57 104039 120 55 104159 240 53 104399 120 55 104519 120 58 104759 240 57 104999 120 55 105119 240 57 105359 120 58 105479 120 57 105599 540 57 106559 360 55 106919 120 53 107039 240 55 107279 120 57 107399 120 57 107519 360 57 108479 360 55 108839 120 53 108959 240 55 109199 120 57 109319 120 57 109439 480 58 110519 240 57 110759 120 55 110879 240 53 111119 120 55 111239 120 58 111359 240 58 111599 120 57 111719 120 55 111839 240 57 112079 120 58 112199 120 57 112319 540 5703";
    std::stringstream stream(data);
    VxFile vxFile(stream);
    vxFile.removeSilenceSpaceFromBeginning();

    VxFileAudioDataGeneratorConfig config;
    config.sampleRate = 44100;
    config.outBufferSize = 256;
    VxFileAudioDataGenerator generator(new PitchRenderer1(config.sampleRate), vxFile, config);
    while (generator.renderNextPitchIfPossible());

    short temp[256];
    std::vector<short> result;
    int count = 0;
    while (result.size() < generator.getFullyFilledPcmDataSize()) {
        int size = generator.readNextSamplesBatch(temp);
        assert(size >= 0);
        count++;
        result.insert(result.end(), temp, temp + size);
    }
}