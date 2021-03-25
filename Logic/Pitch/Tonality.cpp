//
// Created by Semyon Tikhonenko on 3/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#include "Tonality.h"
#include "StringUtils.h"
#include "Pitch.h"

using namespace CppUtils;

static const char* MINOR = "min";
static const char* MAJOR = "maj";

Tonality::Tonality(int pitchInOctaveIndex, bool major) : pitchInOctaveIndex(pitchInOctaveIndex), major(major) {
}

int Tonality::getPitchInOctaveIndex() const {
    return pitchInOctaveIndex;
}

bool Tonality::isMajor() const {
    return major;
}

Tonality::Tonality() {
}

Tonality Tonality::parse(const std::string &str) {
    auto split = Strings::Split(str, ' ');
    if (split.size() != 2) {
        throw std::runtime_error("Tonality::parse failed, Invalid string");
    }

    int pitchInOctaveIndex = Pitch::getPitchInOctaveIndexFromName(split[0]);
    if (pitchInOctaveIndex < 0) {
        throw std::runtime_error("Tonality::parse failed, Invalid pitch");
    }

    const std::string& tonalityStr = split[1];
    bool major;
    if (tonalityStr == MINOR) {
        major = false;
    } else if (tonalityStr == MAJOR) {
        major = true;
    } else {
        std::runtime_error("Tonality::parse failed, Invalid tonality");
    }

    return Tonality(pitchInOctaveIndex, major);
}
