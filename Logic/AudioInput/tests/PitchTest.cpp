#ifndef PITCH_DETECTION_NO_CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../CppUtils/tests/catch.hpp"
#include "Logic/Pitch.h"

TEST_CASE("perfect frequency detection") {
    // near lower bound
    Pitch pitch(127.2);
    REQUIRE(fabs(pitch.getPerfectFrequency() - 130.8) < 0.01);
    REQUIRE(pitch.getOctave() == 3);
    REQUIRE(pitch.getNameAsStdString() == "C");
    float distanceToPerfectFrequency = pitch.getDistanceToPerfectFrequency();
    REQUIRE(distanceToPerfectFrequency > -1.0f);
    REQUIRE(distanceToPerfectFrequency < -0.95f);
    REQUIRE(pitch.hasPerfectFrequency());

    // near upper bound
    pitch = Pitch(134.6);
    REQUIRE(fabs(pitch.getPerfectFrequency() - 130.8) < 0.01);
    REQUIRE(pitch.getOctave() == 3);
    REQUIRE(pitch.getNameAsStdString() == "C");
    distanceToPerfectFrequency = pitch.getDistanceToPerfectFrequency();
    REQUIRE(distanceToPerfectFrequency < 1.0f);
    REQUIRE(distanceToPerfectFrequency > 0.95f);
    REQUIRE(pitch.hasPerfectFrequency());

    // near perfect
    pitch = Pitch(130.8);
    REQUIRE(fabs(pitch.getPerfectFrequency() - 130.8) < 0.01);
    REQUIRE(pitch.getOctave() == 3);
    REQUIRE(pitch.getNameAsStdString() == "C");
    distanceToPerfectFrequency = pitch.getDistanceToPerfectFrequency();
    REQUIRE(distanceToPerfectFrequency < 0.01f);
    REQUIRE(distanceToPerfectFrequency > -0.01f);
    REQUIRE(pitch.hasPerfectFrequency());

    // in the middle between bounds
    pitch = Pitch(130.8);
    REQUIRE(fabs(pitch.getPerfectFrequency() - 130.8) < 0.01);
    REQUIRE(pitch.getOctave() == 3);
    REQUIRE(pitch.getNameAsStdString() == "C");
    distanceToPerfectFrequency = pitch.getDistanceToPerfectFrequency();
    REQUIRE(distanceToPerfectFrequency < 0.01f);
    REQUIRE(distanceToPerfectFrequency > -0.01f);
    REQUIRE(pitch.hasPerfectFrequency());

    pitch = Pitch(-1.0f);
    REQUIRE(!pitch.hasPerfectFrequency());
    REQUIRE(pitch.getPerfectFrequency() < 0.0f);
    REQUIRE(pitch.getOctave() == -1);
    REQUIRE(pitch.getNameAsStdString() == "Invalid");

    pitch = Pitch(123124.5f);
    REQUIRE(!pitch.hasPerfectFrequency());
    REQUIRE(pitch.getPerfectFrequency() < 0.0f);
    REQUIRE(pitch.getOctave() == -1);
    REQUIRE(pitch.getNameAsStdString() == "Invalid");

    pitch = Pitch(10.5f);
    REQUIRE(!pitch.hasPerfectFrequency());
    REQUIRE(pitch.getPerfectFrequency() < 0.0f);
    REQUIRE(pitch.getOctave() == -1);
    REQUIRE(pitch.getNameAsStdString() == "Invalid");
}

TEST_CASE("sound font2 test") {
    // near lower bound
    Pitch pitch(265);
    REQUIRE(pitch.getSoundFont2Index() == 60);
}

static void TestPitchFromName(const char* fullName, const char* expectedName, double frequency, int octave) {
    Pitch pitch(fullName);
    REQUIRE(pitch.isPerfect());
    REQUIRE(pitch.getNameAsStdString() == expectedName);
    REQUIRE(pitch.getOctave() == octave);
    REQUIRE(abs(pitch.getFrequency() - frequency) < 0.00001);
}

TEST_CASE("pitch from name") {
    TestPitchFromName("C3", "C", 130.8, 3);
    TestPitchFromName("C#3", "C#", 138.6, 3);
    TestPitchFromName("Db3", "C#", 138.6, 3);
    TestPitchFromName("D3", "D", 146.8, 3);
    TestPitchFromName("D#3", "Eb", 155.6, 3);
    TestPitchFromName("Eb3", "Eb", 155.6, 3);
    TestPitchFromName("E3", "E", 164.8, 3);
    TestPitchFromName("F3", "F", 174.6, 3);
    TestPitchFromName("F#3", "F#", 185.0, 3);
    TestPitchFromName("G3", "G", 196.0, 3);
    TestPitchFromName("G#3", "G#", 207.7, 3);
    TestPitchFromName("A3", "A", 220.0, 3);
    TestPitchFromName("Bb3", "Bb", 233.1, 3);
    TestPitchFromName("B3", "B", 246.9, 3);
}

