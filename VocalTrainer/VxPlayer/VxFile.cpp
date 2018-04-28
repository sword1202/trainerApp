#include <boost/assert.hpp>
#include "VxFile.h"
#include "GetSf2FilePath.h"
#include "WAVFile.h"
#include "AudioUtils.h"
#include "Strings.h"
#include "Algorithms.h"
#include <utf8.h>
#include <boost/icl/interval_map.hpp>
#include "../../PitchDetection/CppUtils/config.h"

constexpr char SILENCE_MARK[] = "*";
constexpr char LYRICS_START[] = "lyricsStart*";
constexpr char LYRICS_END[] = " lyricsEnd*";
constexpr char NO_LYRICS[] = "noLyrics*";

using namespace CppUtils;

int VxPitch::endTickNumber() const {
    return startTickNumber + ticksCount;
}

VxFile::VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond)
        : pitches(pitches), ticksPerSecond(ticksPerSecond),
          distanceInTicksBetweenLastPitchEndAndTrackEnd(distanceInTicksBetweenLastPitchEndAndTrackEnd) {
    postInit();
}

void VxFile::processLyrics(const std::string& lyricsData) {
    std::vector<int> bracketsPositions;
    size_t size = lyricsData.size();
    bracketsPositions.reserve(size);
    for (int i = 0; i < size; ++i) {
        char ch = lyricsData[i];
        if (ch == '<' || ch == '>') {
            bracketsPositions.push_back(i);
        }
    }

    size_t bracketsPositionsSize = bracketsPositions.size();
    int lyricsAppendingBegin = 0;
    VxLyricsInterval lyricsInterval;
    for (int i = 0; i < bracketsPositionsSize; i+=2) {
        int firstBracketIndex = bracketsPositions[i];
        int secondBracketIndex = bracketsPositions[i + 1];

        bool success;
        auto split = Strings::SplitIntegers(lyricsData, firstBracketIndex + 1, secondBracketIndex, ',', &success);
        if (!success || split.size() != 2) {
            throw new std::runtime_error("Invalid vxFile, unable to parse lyrics");
        }

        lyricsInterval.startTimestampInMilliseconds = split[0];
        lyricsInterval.endTimestampInMilliseconds = split[1];

        lyricsInterval.lyrics.resize(0);
        utf8::utf8to16(lyricsData.begin() + lyricsAppendingBegin,
                lyricsData.begin() + firstBracketIndex,
                std::back_inserter(lyricsInterval.lyrics));

        lyricsAppendingBegin = secondBracketIndex + 1;

        lyrics.push_back(lyricsInterval);
    }
}

VxFile::VxFile(std::istream &is) {
    std::string lyricsState;
    is >> lyricsState;
    // skip space
    is.get();
    if (lyricsState == LYRICS_START) {
        std::string lyricsData = Strings::ReadUntilTokenOrEof(is, LYRICS_END);
        processLyrics(lyricsData);
    } else if(lyricsState != NO_LYRICS) {
        throw std::runtime_error("Invalid vx file");
    }

    is >> ticksPerSecond;

    std::string pitchName;
    while (!is.eof()) {
        is >> pitchName;
        if (pitchName == SILENCE_MARK) {
            is >> distanceInTicksBetweenLastPitchEndAndTrackEnd;
            break;
        }

        VxPitch vxPitch;
        vxPitch.pitch = Pitch(pitchName.data());
        if (!vxPitch.pitch.hasPerfectFrequency()) {
            throw std::runtime_error("Error while parsing pitch with " + pitchName + " name");
        }

        is >> vxPitch.startTickNumber;
        is >> vxPitch.ticksCount;

        pitches.push_back(vxPitch);
    }

    postInit();
    BOOST_ASSERT(validateLyrics());
}

static inline size_t addSilence(std::vector<char>& pcmData, double duration, int sampleRate) {
    size_t size = static_cast<size_t>((int)round(duration * sampleRate) * sizeof(short));
    pcmData.resize(pcmData.size() + size);
    return size;
}

double VxFile::getTickDurationInSeconds() const {
    return 1.0 / (double) ticksPerSecond;
}

bool VxFile::validatePitches() {
    if (!pitches.empty()) {
        if (pitches[0].startTickNumber < 0) {
            return false;
        }
        
        if (pitches[0].ticksCount < 1) {
            return false;
        }
    }
    
    for (int i = 1; i < pitches.size(); ++i) {
        const VxPitch &vxPitch = pitches[i];
        if (!vxPitch.pitch.hasPerfectFrequency()) {
            return false;
        }
        
        if (vxPitch.ticksCount < 1) {
            return false;
        }
    }

    return true;
}

void VxFile::postInit() {
    SortByKey(pitches, startTickNumberKeyProvider);

    BOOST_ASSERT(distanceInTicksBetweenLastPitchEndAndTrackEnd >= 0);
    BOOST_ASSERT(validatePitches());
    if (!pitches.empty()) {
        const VxPitch &lastPitch = pitches.back();
        durationInTicks = lastPitch.startTickNumber + lastPitch.ticksCount + distanceInTicksBetweenLastPitchEndAndTrackEnd;
    }
}

double VxFile::getDurationInSeconds() const {
    return getTickDurationInSeconds() * durationInTicks;
}

const std::vector<VxPitch> &VxFile::getPitches() const {
    return pitches;
}

int VxFile::getTicksPerSecond() const {
    return ticksPerSecond;
}

int VxFile::getDurationInTicks() const {
    return durationInTicks;
}

int VxFile::getDistanceInTicksBetweenLastPitchEndAndTrackEnd() const {
    return distanceInTicksBetweenLastPitchEndAndTrackEnd;
}

void VxFile::writeToStream(std::ostream &os) const {
    if (lyrics.empty()) {
        os<<NO_LYRICS;
    } else {
        os<<LYRICS_START<<" ";
        for (const auto& interval : lyrics) {
            std::string utf8lyrics;
            utf8::utf16to8(interval.lyrics.begin(), interval.lyrics.end(), std::back_inserter(utf8lyrics));
            os<<utf8lyrics<<'<'<<interval.startTimestampInMilliseconds<<','<<interval.endTimestampInMilliseconds<<'>';
        }
        os<<LYRICS_END;
    }

    os<<" "<<ticksPerSecond<<" ";

    for (const VxPitch& vxPitch : pitches) {
        os<<vxPitch.pitch.getFullName()<<" "<<vxPitch.startTickNumber<<" "<<vxPitch.ticksCount<<" ";
    }

    os<<"* "<<distanceInTicksBetweenLastPitchEndAndTrackEnd;
}

VxFile VxFile::fromFilePath(const char *filePath) {
    std::ifstream is(filePath);
    return VxFile(is);
}

void VxFile::setLyrics(const std::vector<VxLyricsInterval> &lyrics) {
    this->lyrics = lyrics;
    BOOST_ASSERT(validateLyrics());
}

bool VxFile::validateLyrics() {
    if (lyrics.empty()) {
        return true;
    }

    if (lyrics[0].startTimestampInMilliseconds < 0) {
        return false;
    }

    if (lyrics[0].endTimestampInMilliseconds < 1) {
        return false;
    }

    if (lyrics[0].endTimestampInMilliseconds < lyrics[0].startTimestampInMilliseconds) {
        return false;
    }

    for (int i = 1; i < lyrics.size(); ++i) {
        const VxLyricsInterval &interval = lyrics[i];
        const VxLyricsInterval &prev = lyrics[i - 1];

        if (interval.startTimestampInMilliseconds < 0) {
            return false;
        }

        if (interval.endTimestampInMilliseconds <= interval.startTimestampInMilliseconds) {
            return false;
        }

        if (interval.startTimestampInMilliseconds < prev.endTimestampInMilliseconds) {
            return false;
        }
    }

    if (lyrics.back().endTimestampInMilliseconds > getDurationInSeconds() * 1000) {
        return false;
    }

    return true;
}

const std::vector<VxLyricsInterval> &VxFile::getLyrics() const {
    return lyrics;
}

int VxFile::timeInSecondsToTicks(double timeInSeconds)const {
    return static_cast<int>(round(timeInSeconds / getTickDurationInSeconds()));
}

double VxFile::ticksToSeconds(int ticks) const {
    return ticks * getTickDurationInSeconds();
}

int VxFile::samplesCountFromTicks(int ticks, int sampleRate) const {
    return (int) round(ticksToSeconds(ticks) * sampleRate);
}

int VxFile::ticksFromSamplesCount(int samplesCount, int sampleRate) const {
    return timeInSecondsToTicks(AudioUtils::GetSampleTimeInSeconds(samplesCount, sampleRate));
}

int VxFile::startTickNumberKeyProvider(const VxPitch &pitch) {
    return pitch.startTickNumber;
}
