//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "VxFile.h"
#include <fstream>
#include <thread>
#include <iostream>
#include "../json/reader.h"
#include "WavAudioDataFromPitchGenerator.h"
#include "TimeUtils.h"
#include "Algorithms.h"

#define LOCK_CURRENT_SEEK std::lock_guard<std::mutex> _(currentSeekMutex)

using namespace CppUtils;
using std::cout;

static bool ComparePitches(const VxPitchDefinition& a, const VxPitchDefinition& b) {
    return a.timestamp < b.timestamp;
}

VxPitchDefinition::VxPitchDefinition(const Pitch &pitch, double timestamp) : pitch(pitch), timestamp(timestamp) {}
VxPitchDefinition::VxPitchDefinition() {}

VxFile::VxFile(const std::vector<VxPitchDefinition> &pitches) : pitches(pitches) {
}

void VxFile::load(std::istream &stream) {
    initFromStream(stream);
}

void VxFile::load(const char *fileName) {
    std::ifstream file(fileName);
    initFromStream(file);
}

void VxFile::initFromStream(std::istream &stream) {
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(stream, root, false)) {
        throw std::runtime_error("json parse failed, " + reader.getFormattedErrorMessages());
    }
    Json::Value pitches = root["pitches"];
    if (!pitches.isObject()) {
        throw std::runtime_error("the file has no pitches key or pitches is not an object");
    }
    
    for (const std::string& key : pitches.getMemberNames()) {
        const Json::Value &value = pitches[key];
        if (value.isNull()) {
            double timestamp = strtod(key.data(), 0);
            this->pitches.push_back(VxPitchDefinition(Pitch(), timestamp));
            continue;
        } else if (!value.isString()) {
            continue;
        }

        const char* pitchName = value.asCString();
        Pitch pitch(pitchName);
        if (pitch.isValid()) {
            double timestamp = strtod(key.data(), 0);
            this->pitches.push_back(VxPitchDefinition(pitch, timestamp));
        }
    }

    std::sort(this->pitches.begin(), this->pitches.end(), ComparePitches);

    // the file should always end at silence
    if (!this->pitches.empty()) {
        VxPitchDefinition &back = this->pitches.back();
        if (back.pitch.isValid()) {
            back.pitch = Pitch();
        }
    }

    WavAudioDataFromPitchGenerator generator;
    for (int i = 0; i < this->pitches.size() - 1; ++i) {
        VxPitchDefinition &current = this->pitches[i];
        VxPitchDefinition &next = this->pitches[i + 1];
        double duration = next.timestamp - current.timestamp;
        current.duration = duration;
        if (current.pitch.isValid()) {
            current.audioData = generator.pitchToWavAudioData(current.pitch, duration);
        }
    }

    player = new AudioPlayer();
}

VxFile::~VxFile() {
    if (player) {
        delete player;
    }
}

void VxFile::play() {
    if (!isPlaying) {
        isPlaying = true;
        currentSeek = 0;
        std::thread thread([this] {
            int64_t time = TimeUtils::NowInMicroseconds();
            VxPitchDefinition searchPlaceholder;
            double startPlayCurrentSeek = 0;
            double lastDuration = 0;
            while (isPlaying) {
                double currentSeek;
                {
                    LOCK_CURRENT_SEEK;
                    currentSeek = this->currentSeek;
                }

                std::this_thread::sleep_for(std::chrono::microseconds(10));
                int64_t now = TimeUtils::NowInMicroseconds();
                if (!isPaused) {
                    currentSeek += (now - time) / 1000000.0;
                }
                time = now;

                if (currentSeek - startPlayCurrentSeek >= lastDuration) {
                    searchPlaceholder.timestamp = currentSeek;
                    auto iter = CppUtils::FindLessOrEqualInSortedCollection(pitches, searchPlaceholder, ComparePitches);

                    if (iter != pitches.end() && iter->pitch.isValid()) {
                        startPlayCurrentSeek = currentSeek;
                        lastDuration = iter->duration;
                        player->play(iter->audioData.data(), iter->audioData.size(), currentSeek - iter->timestamp);
                    }
                }
            }
        });
        thread.detach();
    } else if (isPaused) {
        player->resume();
    }
}

void VxFile::stop() {
    isPlaying = false;
    player->stop();
}

void VxFile::pause() {
    player->pause();
}

void VxFile::seek(double timeStamp) {
    player->stop();
    LOCK_CURRENT_SEEK;
    currentSeek = timeStamp;
}

void VxFile::reset() {
    stop();
    seek(0);
}

VxFile::VxFile() {

}
