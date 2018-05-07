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
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace CppUtils;

int VxPitch::endTickNumber() const {
    return startTickNumber + ticksCount;
}

VxFile::VxFile(std::vector<VxPitch> &&pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond)
        : pitches(std::move(pitches)),
          ticksPerSecond(ticksPerSecond),
          distanceInTicksBetweenLastPitchEndAndTrackEnd(distanceInTicksBetweenLastPitchEndAndTrackEnd) {
    postInit();
}

VxFile::VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond)
        : pitches(pitches),
          ticksPerSecond(ticksPerSecond),
          distanceInTicksBetweenLastPitchEndAndTrackEnd(distanceInTicksBetweenLastPitchEndAndTrackEnd)
{
    postInit();
}


VxFile::VxFile(std::istream &is) {
    boost::archive::text_iarchive ar(is);
    ar >> *this;

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
    boost::archive::text_oarchive ar(os);
    ar << *this;
}

VxFile VxFile::fromFilePath(const char *filePath) {
    std::ifstream is(filePath);
    return VxFile(is);
}

bool VxFile::validateLyrics() {
    if (lyrics.empty()) {
        return true;
    }

    for (const auto& line : lyrics) {
        const std::vector<VxLyricsInterval> &intervals = line.intervals;
        if (intervals[0].startTimestampInMilliseconds < 0) {
            return false;
        }

        if (intervals[0].endTimestampInMilliseconds < 1) {
            return false;
        }

        if (intervals[0].endTimestampInMilliseconds < intervals[0].startTimestampInMilliseconds) {
            return false;
        }

        for (int i = 1; i < lyrics.size(); ++i) {
            const VxLyricsInterval &interval = intervals[i];
            const VxLyricsInterval &prev = intervals[i - 1];

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

        if (intervals.back().endTimestampInMilliseconds > getDurationInSeconds() * 1000) {
            return false;
        }
    }

    return true;
}

const VxLyricsLine &VxFile::getLyricsLine(int lineIndex) const {
    return lyrics[lineIndex];
}

int VxFile::getLyricsLinesCount() const {
    return (int)lyrics.size();
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

const VxPitch &VxFile::getShortestPitch() const {
    return FindMinValueUsingKeyProvider(pitches, [](const VxPitch& pitch) {
        return pitch.ticksCount;
    });
}

void VxFile::addLyricsLine(const VxLyricsLine &line) {
    lyrics.push_back(line);
}

VxFile::VxFile() {

}

void VxFile::setPitches(const std::vector<VxPitch> &pitches) {
    VxFile::pitches = pitches;
    postInit();
}