//
// Created by Semyon Tikhonenko on 8/7/16.
//

#include "Pitch.h"

#include <iostream>
#include <assert.h>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <unordered_map>
#include "Maps.h"

#ifndef NDEBUG
#define DEBUG_INIT fullName = getFullName();
#else
#define DEBUG_INIT
#endif

using namespace CppUtils;

static const int SOUND_FONT2_INDEX_DIFF = 24;
static const int MIDI_PITCH_INDEX_OFFSET = 24;

// C1-B7 perfect frequencies
static const float FREQUENCIES[] = {
                        /*C#*/  /*D*/   /*D#*/   /*E*/  /*F*/   /*F#*/  /*G*/   /*G#*/  /*A*/   /*A#*/  /*B*/
        /*C1*/ 32.70,	34.65,	36.71,	38.89,	41.20,	43.65,	46.25,	49.00,	51.91,	55.00,	58.27,	61.74,
        /*C2*/ 65.41,	69.30,	73.42,	77.78,	82.41,	87.31,	92.50,	98.00,	103.8,	110.0,	116.5,	123.5,
        /*C3*/ 130.8,	138.6,	146.8,	155.6,	164.8,	174.6,	185.0,	196.0,	207.7,	220.0,	233.1,	246.9,
        /*C4*/ 261.6,	277.2,	293.7,	311.1,	329.6,	349.2,	370.0,	392.0,	415.3,	440.0,	466.2,	493.9,
        /*C5*/ 523.3,	554.4,	587.3,	622.3,	659.3,	698.5,	740.0,	784.0,	830.6,	880.0,	932.3,	987.8,
        /*C6*/ 1047,	1109,	1175,	1245,	1319,	1397,	1480,	1568,	1661,	1760,	1865,	1976,
        /*C7*/ 2093,	2217,	2349,	2489,	2637,	2794,	2960,	3136,	3322,	3520,	3729,	3951
};

// Pitches lower and upper bounds. For example C3 has (127.1, 134.7) bounds, but the perfect frequency is 130.8
static const float PITCHES_BOUNDS_FREQUENCIES[] = {
        31.77,	33.66,	35.67,	37.79,	40.03,	42.41,	44.94,	47.61,	50.44,	53.44,	56.62,	59.98,
        63.55,	67.33,	71.33,	75.57,	80.07,	84.83,	89.87,	95.22,	100.9,	106.9,	113.2,	120.0,
        127.1,	134.7,	142.7,	151.1,	160.1,	169.7,	179.7,	190.4,	201.8,	213.8,	226.5,	239.9,
        254.2,	269.3,	285.3,	302.3,	320.3,	339.3,	359.5,	380.9,	403.5,	427.5,	452.9,	479.9,
        508.4,	538.6,	570.7,	604.6,	640.5,	678.6,	719.0,	761.7,	807.0,	855.0,	905.9,	959.7,
        1016.8,	1077.2,	1141.3,	1209.2,	1281.1,	1357.2,	1438.0,	1523.5,	1614.0,	1710.0,	1811.7,	1919.4,
        2033.6,	2154.5,	2282.6,	2418.3,	2562.1,	2714.5,	2875.9,	3046.9,	3228.1,	3420.0,	3623.4,	3838.9,
        4067.1,
};

static const int WHITE_PITCH_INDEX_TO_IN_OCTAVE_INDEX[] = {
        0, 2, 4, 5, 7, 9, 11
};

static const int IN_OCTAVE_INDEX_TO_WHITE_PITCH_INDEX[] = {
        0, -1, 1, -1, 2, 3, -1, 4, -1, 5, -1, 6
};

static const int NEXT_WHITE_PITCH_INDEX_OFFSET[] = {
        2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 1
};

static const char* PITCHES[]  = {
        "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb",	"B"
};

// used in distance to next frequency calculation A = 2**(1/24)
static const float A = 1.029302236643492f;

static const float log2A = log2f(A);

float Pitch::getFrequency() const {
    return frequency;
}

Pitch::Pitch(float frequency) : frequency(frequency) {
    if (frequency < 0) {
        perfectFrequencyIndex = -1;
        return;
    }

    const float* end = FREQUENCIES + FREQUENCIES_COUNT;
    const float* frequencyLowerBound = std::lower_bound(FREQUENCIES, end, frequency);
    if (frequencyLowerBound == end) {
        if (frequency > PITCHES_BOUNDS_FREQUENCIES[FREQUENCIES_COUNT]) {
            perfectFrequencyIndex = -1;
        } else {
            perfectFrequencyIndex = FREQUENCIES_COUNT - 1;
        }
        return;
    }

    int index = (int) (frequencyLowerBound - FREQUENCIES);
    const float * perfectFrequencyPointer = frequencyLowerBound;
    if (frequency < PITCHES_BOUNDS_FREQUENCIES[index]) {
        perfectFrequencyPointer--;
    }

    perfectFrequencyIndex = (int) (perfectFrequencyPointer - FREQUENCIES);

    DEBUG_INIT
}

const char *Pitch::getName() const {
    if (perfectFrequencyIndex < 0) {
        return "Invalid";
    }

    return PITCHES[perfectFrequencyIndex % PITCHES_IN_OCTAVE];
}

float Pitch::getPerfectFrequency() const {
    if (perfectFrequencyIndex < 0) {
        return -1.0f;
    }

    return FREQUENCIES[perfectFrequencyIndex];
}

int Pitch::getOctave() const {
    if (perfectFrequencyIndex < 0) {
        return -1;
    }

    return perfectFrequencyIndex / PITCHES_IN_OCTAVE + FIRST_SUPPORTED_OCTAVE;
}

bool Pitch::hasPerfectFrequency() const {
    return perfectFrequencyIndex >= 0;
}

float Pitch::getDistanceToPerfectFrequency() const {
    return getDistanceFromLowerBound() - 1.0f;
}

float Pitch::getDistanceFromLowerBound() const {
    assert(hasPerfectFrequency());
    float lowerBound = PITCHES_BOUNDS_FREQUENCIES[perfectFrequencyIndex];
    return log2f(frequency / lowerBound) / log2A;
}

std::string Pitch::getNameAsStdString() const {
    return getName();
}

Pitch::Pitch(float frequency, int perfectFrequencyIndex) {
    this->frequency = frequency;
    this->perfectFrequencyIndex = perfectFrequencyIndex;
    assert(perfectFrequencyIndex >= 0);
    assert(perfectFrequencyIndex < FREQUENCIES_COUNT);
    assert(frequency >= PITCHES_BOUNDS_FREQUENCIES[perfectFrequencyIndex]);
    assert(frequency <= PITCHES_BOUNDS_FREQUENCIES[perfectFrequencyIndex + 1]);

    DEBUG_INIT
}

int Pitch::getPerfectFrequencyIndex() const {
    return perfectFrequencyIndex;
}

Pitch::Pitch() {
    frequency = -1;
    perfectFrequencyIndex = -1;

    DEBUG_INIT
}

int Pitch::getPitchInOctaveIndex() const {
    return perfectFrequencyIndex % PITCHES_IN_OCTAVE;
}

float Pitch::getLowerBoundFrequencyOfPitch(int pitchInOctaveIndex, int octaveIndex) {
    return PITCHES_BOUNDS_FREQUENCIES[octaveIndex * PITCHES_IN_OCTAVE + pitchInOctaveIndex];
}

float Pitch::getUpperBoundFrequencyOfPitch(int pitchInOctaveIndex, int octaveIndex) {
    return PITCHES_BOUNDS_FREQUENCIES[octaveIndex * PITCHES_IN_OCTAVE + pitchInOctaveIndex + 1];
}

bool Pitch::isValid() const {
    return hasPerfectFrequency();
}

bool Pitch::isWhite() const {
    int pitchInOctaveIndex = getPitchInOctaveIndex();
    if (pitchInOctaveIndex < 0) {
        return false;
    }

    return isWhite(pitchInOctaveIndex);
}

bool Pitch::isWhite(int pitchInOctaveIndex) {
    assert(pitchInOctaveIndex >= 0 && pitchInOctaveIndex < PITCHES_IN_OCTAVE);
    return IN_OCTAVE_INDEX_TO_WHITE_PITCH_INDEX[pitchInOctaveIndex] >= 0;
}

bool Pitch::isBlack() const {
    int pitchInOctaveIndex = getPitchInOctaveIndex();
    if (pitchInOctaveIndex < 0) {
        return false;
    }

    return !isWhite(pitchInOctaveIndex);
}

int Pitch::getWhitePitchIndexFromInOctaveIndex(int pitchInOctaveIndex) {
    assert(pitchInOctaveIndex >= 0);
    if (pitchInOctaveIndex <= 4) {
        return pitchInOctaveIndex / 2;
    } else {
        return (pitchInOctaveIndex + 1) / 2;
    }
}

int Pitch::getBlackPitchIndexFromInOctaveIndex(int pitchInOctaveIndex) {
    assert(!isWhite(pitchInOctaveIndex));
    if (pitchInOctaveIndex <= 4) {
        return pitchInOctaveIndex / 2;
    } else {
        return (pitchInOctaveIndex - 1) / 2;
    }
}

int Pitch::getSoundFont2Index() const {
    assert(hasPerfectFrequency());
    return perfectFrequencyIndex + SOUND_FONT2_INDEX_DIFF;
}

Pitch::Pitch(const char *name) {
    size_t len = strlen(name);
    if (len == 0) {
        frequency = -1;
        perfectFrequencyIndex = -1;
        return;
    }
    
    int octave = name[len - 1] - '0';
    if (octave < FIRST_SUPPORTED_OCTAVE || octave > OCTAVES_COUNT + FIRST_SUPPORTED_OCTAVE - 1) {
        frequency = -1;
        perfectFrequencyIndex = -1;
        return;
    }

    int pitchInOctaveIndex = 0;
    if (len == 2 || len == 3) {
        switch (name[0]) {
            case 'C':
                pitchInOctaveIndex = C_INDEX;
                break;
            case 'D':
                pitchInOctaveIndex = D_INDEX;
                break;
            case 'E':
                pitchInOctaveIndex = E_INDEX;
                break;
            case 'F':
                pitchInOctaveIndex = F_INDEX;
                break;
            case 'G':
                pitchInOctaveIndex = G_INDEX;
                break;
            case 'A':
                pitchInOctaveIndex = A_INDEX;
                break;
            case 'B':
                pitchInOctaveIndex = B_INDEX;
                break;
            default:
                frequency = -1;
                perfectFrequencyIndex = -1;
                return;
        }

        if (len == 3) {
            if (name[1] == 'b') {
                pitchInOctaveIndex--;
            } else if(name[1] == '#') {
                pitchInOctaveIndex++;
            }
        }

        if (pitchInOctaveIndex < 0 || pitchInOctaveIndex >= PITCHES_IN_OCTAVE) {
            frequency = -1;
            perfectFrequencyIndex = -1;
            return;
        }

        perfectFrequencyIndex = (octave - FIRST_SUPPORTED_OCTAVE) * PITCHES_IN_OCTAVE + pitchInOctaveIndex;
        frequency = FREQUENCIES[perfectFrequencyIndex];

    } else {
        frequency = -1;
        perfectFrequencyIndex = -1;
    }

    DEBUG_INIT
}

bool Pitch::isPerfect() const {
    if (!hasPerfectFrequency()) {
        return false;
    }

    return abs(frequency - FREQUENCIES[perfectFrequencyIndex]) < 0.00001;
}

int Pitch::getMidiIndex() const {
    assert(hasPerfectFrequency());
    return MIDI_PITCH_INDEX_OFFSET + perfectFrequencyIndex;
}

Pitch Pitch::fromPerfectFrequencyIndex(int perfectFrequencyIndex) {
    if (perfectFrequencyIndex < 0 || perfectFrequencyIndex >= FREQUENCIES_COUNT) {
        return Pitch();
    }

    return Pitch(FREQUENCIES[perfectFrequencyIndex], perfectFrequencyIndex);
}

Pitch Pitch::fromMidiIndex(int midiIndex) {
    return fromPerfectFrequencyIndex(midiIndex - MIDI_PITCH_INDEX_OFFSET);
}

std::string Pitch::getFullName() const {
    return getNameAsStdString() + std::to_string(getOctave());
}

void Pitch::shift(int distance) {
    assert(canBeShifted(distance));
    perfectFrequencyIndex += distance;
}

bool Pitch::canBeShifted(int distance) const {
    int resultPitchIndex = distance + perfectFrequencyIndex;
    return resultPitchIndex >= 0 && resultPitchIndex < FREQUENCIES_COUNT;
}

bool Pitch::isMidiPitchSupported(int midiIndex) {
    const int resultIndex = midiIndex - MIDI_PITCH_INDEX_OFFSET;
    return resultIndex >= 0 && resultIndex < FREQUENCIES_COUNT;
}

Pitch Pitch::whitePitch(int whitePitchInOctaveIndex, int octaveIndex) {
    int perfectFrequencyIndex = whitePitchPerfectFrequencyIndex(whitePitchInOctaveIndex, octaveIndex);
    return Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex);
}

int Pitch::whitePitchPerfectFrequencyIndex(int whitePitchInOctaveIndex, int octaveIndex) {
    assert(octaveIndex >= FIRST_SUPPORTED_OCTAVE && octaveIndex < FIRST_SUPPORTED_OCTAVE + OCTAVES_COUNT);
    int perfectFrequencyIndex = WHITE_PITCH_INDEX_TO_IN_OCTAVE_INDEX[whitePitchInOctaveIndex]
            + octaveIndex * PITCHES_IN_OCTAVE;
    return perfectFrequencyIndex;
}

Pitch Pitch::ofOctave(int octave, int inOctaveIndex) {
    assert(octave >= FIRST_SUPPORTED_OCTAVE);
    assert(octave < FIRST_SUPPORTED_OCTAVE + OCTAVES_COUNT);
    assert(inOctaveIndex >= 0 && inOctaveIndex < PITCHES_IN_OCTAVE);
    return Pitch::fromPerfectFrequencyIndex(octave * PITCHES_IN_OCTAVE + inOctaveIndex);
}

int Pitch::getWhiteIndex() const {
    assert(perfectFrequencyIndex >= 0);
    return IN_OCTAVE_INDEX_TO_WHITE_PITCH_INDEX[getPitchInOctaveIndex()];
}

Pitch Pitch::getNextWhitePitch() const {
    return Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex + NEXT_WHITE_PITCH_INDEX_OFFSET[getPitchInOctaveIndex()]);
}

int Pitch::getPitchInOctaveIndexFromName(const std::string& name) {
    static std::unordered_map<std::string, int> map = ([&] {
        static std::unordered_map<std::string, int> result;
        for (int i = 0; i < PITCHES_IN_OCTAVE; ++i) {
            result[PITCHES[i]] = i;
        }

        return result;
    })();

    return Maps::GetOrDefault(map, name, -1);
}


std::ostream& operator<<(std::ostream& os, const Pitch& pitch) {
    os<<pitch.getName()<<pitch.getOctave()<<" ("<<pitch.getFrequency()<<")";
    return os;
}

std::istream& operator>>(std::istream& is, Pitch& pitch) {
    std::string pitchAsString;
    is >> pitchAsString;
    pitch = Pitch(pitchAsString.data());
    return is;
}
