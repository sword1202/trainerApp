//
// Created by Semyon Tykhonenko on 1/19/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "VxFile.h"
#include "config.h"
#include "Algorithms.h"
#include "PitchDuration.h"
#include <optional>
#include "MathUtils.h"
#include "StringUtils.h"
#include "Collections.h"
#include "Maps.h"
#include "NotImplementedAssert.h"

using namespace CppUtils;
using namespace Math;

void VxFile::readFromStream(std::istream &is) {
    std::string signature;
    is >> signature;
    if (signature != "VX") {
        throw std::runtime_error("Unsupported file, wrong signature");
    }

    int version;
    is >> version;

    is >> beatsPerMinute;
    if (beatsPerMinute <= 0) {
        throw std::runtime_error("invalid beatsPerMinute");
    }

    std::vector<std::optional<Pitch>> pitches;
    pitches.reserve(200);
    std::vector<int> durations;
    durations.reserve(200);

    bool isPitch = true;
    while (!is.eof()) {
        if (isPitch) {
            std::string pitchName;
            is >> pitchName;
            // Check if pause
            if (pitchName == "P") {
                pitches.push_back(std::nullopt);
            } else {
                Pitch pitch(pitchName.data());
                pitches.push_back(pitch);
            }
        } else {
            int duration;
            is >> duration;
            if (!PitchDuration::isValid(duration)) {
                throw std::runtime_error("Invalid pitch duration");
            }
            durations.push_back(duration);
        }
        isPitch = !isPitch;
    }

    if (pitches.empty()) {
        this->vocalPart = VocalPart(std::vector<NoteInterval>(), 0, beatsPerMinute / 60.0);
        return;
    }

    if (durations.size() != pitches.size()) {
        throw std::runtime_error("All pitches should have corresponding durations");
    }

    int ticksPerBeat = PitchDuration::getRequiredTicksPerBeat(*MaxByKey(durations, [] (int duration) {
        return PitchDuration::getRequiredTicksPerBeat(duration);
    }));

    double ticksPerSecond = (beatsPerMinute / 60.0) * ticksPerBeat;

    std::vector<NoteInterval> intervals;
    int currentTick = 0;
    int lastPitchEndTick = 0;
    intervals.reserve(pitches.size());
    for (int i = 0; i < pitches.size(); ++i) {
        std::optional<Pitch> pitch = pitches[i];
        int duration = durations[i];
        int ticksCount = RoundToInt(PitchDuration::getBeatsCount(duration) * ticksPerBeat);
        if (pitch) {
            NoteInterval interval(*pitch, currentTick, ticksCount);
            lastPitchEndTick = currentTick + ticksCount;
            intervals.push_back(interval);
        }
        currentTick += ticksCount;
    }

    int distanceInTicksBetweenLastPitchEndAndTrackEnd = currentTick - lastPitchEndTick;
    this->vocalPart = VocalPart(std::move(intervals), distanceInTicksBetweenLastPitchEndAndTrackEnd, ticksPerSecond);
}

void VxFile::parse(const std::string &source) {
    std::istringstream is(source);
    this->readFromStream(is);
}

const VocalPart &VxFile::getVocalPart() const {
    return vocalPart;
}

const std::map<double, int> &VxFile::getRecordingTonalityChanges() const {
    return Maps::emptyMap<double, int>();
}

const Lyrics &VxFile::getLyrics() const {
    return Lyrics::EMPTY;
}

AudioDataBufferConstPtr VxFile::getInstrumental() const {
    return nullptr;
}

bool VxFile::isRecording() const {
    return false;
}

const std::vector<float> &VxFile::getRecordedPitchesFrequencies() const {
    return Collections::emptyVector<float>();
}

const std::vector<double> &VxFile::getRecordedPitchesTimes() const {
    return Collections::emptyVector<double>();
}

AudioDataBufferConstPtr VxFile::getRecordingData() const {
    return nullptr;
}

const std::string &VxFile::getArtistNameUtf8() const {
    return Strings::EMPTY;
}

const std::string &VxFile::getSongTitleUtf8() const {
    return Strings::EMPTY;
}

double VxFile::getBeatsPerMinute() const {
    return beatsPerMinute;
}

const std::vector<short> &VxFile::getInstrumentalPreviewSamples() const {
    return Collections::emptyVector<short>();
}

const Tonality &VxFile::getOriginalTonality() const {
    NOT_IMPLEMENTED_ASSERT;
    return Tonality();
}
