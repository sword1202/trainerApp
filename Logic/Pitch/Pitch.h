//
// Created by Semyon Tikhonenko on 8/7/16.
//

#ifndef PITCHDETECTION_PITCH_H
#define PITCHDETECTION_PITCH_H

#include <string>

class Pitch {
    float frequency;
    int perfectFrequencyIndex;

#ifndef NDEBUG
    std::string fullName;
#endif
public:
    static const int OCTAVES_COUNT = 7;
    static const int FIRST_SUPPORTED_OCTAVE = 1;
    static const int PITCHES_IN_OCTAVE = 12;
    static const int FREQUENCIES_COUNT = OCTAVES_COUNT * PITCHES_IN_OCTAVE;

    static const int C_INDEX = 0;
    static const int C_SHARP_INDEX = 1;
    static const int D_INDEX = 2;
    static const int Eb_INDEX = 3;
    static const int E_INDEX = 4;
    static const int F_INDEX = 5;
    static const int F_SHARP_INDEX = 6;
    static const int G_INDEX = 7;
    static const int G_SHARP_INDEX = 8;
    static const int A_INDEX = 9;
    static const int Bb_INDEX = 10;
    static const int B_INDEX = 11;

    Pitch();
    explicit Pitch(float frequency);
    Pitch(float frequency, int perfectFrequencyIndex);
    Pitch(const char* name);
    static Pitch fromPerfectFrequencyIndex(int perfectFrequencyIndex);
    static Pitch ofOctave(int octaveIndex, int inOctaveIndex);
    static Pitch fromMidiIndex(int midiIndex);
    static Pitch whitePitch(int whitePitchInOctaveIndex, int octaveIndex);
    static int whitePitchPerfectFrequencyIndex(int whitePitchInOctaveIndex, int octaveIndex);
public:
    float getFrequency() const;
    const char* getName() const;
    std::string getNameAsStdString() const;
    std::string getFullName() const;
    float getPerfectFrequency() const;
    int getOctave() const;
    bool hasPerfectFrequency() const;
    bool isValid() const;
    bool isPerfect() const;
    // Calculated using formula: n = log(fn / f0, a), where
    // A = 2**(1/24),
    // f0 - pitch lower bound frequency,
    // fn - pitch actual frequency
    // returns value (-1.0f, 1.0f). Where 0 - perfect frequency
    float getDistanceToPerfectFrequency() const;
    // returns value (0.0f, 2.0f). Where 1.0 - perfect frequency
    float getDistanceFromLowerBound() const;
    int getPerfectFrequencyIndex() const;
    int getPitchInOctaveIndex() const;
    // Get index of the pitch, used in SoundFont2 format
    int getSoundFont2Index() const;
    int getMidiIndex() const;

    bool isWhite() const;
    bool isBlack() const;
    int getWhiteIndex() const;

    void shift(int distance);
    bool canBeShifted(int distance) const;

    Pitch getNextWhitePitch() const;

    static float getLowerBoundFrequencyOfPitch(int pitchInOctaveIndex, int octaveIndex);
    static float getUpperBoundFrequencyOfPitch(int pitchInOctaveIndex, int octaveIndex);

    static bool isWhite(int pitchInOctaveIndex);
    static int getWhitePitchIndexFromInOctaveIndex(int pitchInOctaveIndex);
    static int getBlackPitchIndexFromInOctaveIndex(int pitchInOctaveIndex);

    static int getPitchInOctaveIndexFromName(const std::string& name);

    static bool isMidiPitchSupported(int midiIndex);
};

std::ostream& operator<<(std::ostream& os, const Pitch& pitch);
std::istream& operator>>(std::istream& is, Pitch& pitch);

#endif //PITCHDETECTION_PITCH_H
