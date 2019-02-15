#include <boost/assert.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <utf8.h>

#include "config.h"
#include "VxFile.h"
#include "AudioUtils.h"
#include "StringUtils.h"
#include "Algorithms.h"

using namespace CppUtils;

int Interval::endTickNumber() const {
    return startTickNumber + ticksCount;
}

bool Interval::containsTick(int tick) const {
    return startTickNumber <= tick && endTickNumber() > tick;
}

bool Interval::intersectsWith(int begin, int end) const {
    return intersectsWith(Interval(begin, end - begin));
}

bool Interval::intersectsWith(const Interval &interval) const {
    return containsTick(interval.startTickNumber) || containsTick(interval.endTickNumber() - 1) ||
            interval.containsTick(startTickNumber) || interval.containsTick(endTickNumber() - 1);
}

Interval::Interval(int startTickNumber, int ticksCount) : startTickNumber(startTickNumber), ticksCount(ticksCount) {
}

VxPitch::VxPitch(const Pitch &pitch, int startTickNumber, int ticksCount)
        : Interval(startTickNumber, ticksCount), pitch(pitch) {
}

std::ostream& operator<<(std::ostream& os, const VxPitch& pitch) {
    os<<pitch.pitch<<"["<<pitch.startTickNumber<<","<<pitch.endTickNumber()<<"]";
    return os;
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
    auto pair = FindMinMaxUsingKeyProvider(pitches, [](const VxPitch& vxPitch) {
        return vxPitch.pitch.getPerfectFrequencyIndex();
    });

    lowestPitchIndex = pair.first - pitches.begin();
    highestPitchIndex = pair.second - pitches.begin();

    BOOST_ASSERT(distanceInTicksBetweenLastPitchEndAndTrackEnd >= 0);
    BOOST_ASSERT(validatePitches());
    if (!pitches.empty()) {
        auto lastPitchIter = MaxByKey(pitches, [] (const VxPitch& vxPitch) {
            return vxPitch.endTickNumber();
        });
        durationInTicks = lastPitchIter->endTickNumber() + distanceInTicksBetweenLastPitchEndAndTrackEnd;
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

#ifdef USE_BOOST_SERIALIZATION

void VxFile::writeToStream(std::ostream &os) const {
    boost::archive::text_oarchive ar(os);
    ar << *this;
}

VxFile::VxFile(std::istream &is) {
    boost::archive::text_iarchive ar(is);
    ar >> *this;
    postInit();
}

VxFile VxFile::fromFilePath(const char *filePath) {
    std::ifstream is(filePath);
    return VxFile(is);
}

#endif

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

VxFile::VxFile() {

}

void VxFile::setPitches(const std::vector<VxPitch> &pitches) {
    VxFile::pitches = pitches;
    postInit();
}

bool VxFile::canBeShifted(int distance) const {
    return pitches[lowestPitchIndex].pitch.canBeShifted(distance) &&
            pitches[highestPitchIndex].pitch.canBeShifted(distance);
}

void VxFile::shift(int distance) {
    assert(canBeShifted(distance));
    for (auto& pitch : pitches) {
        pitch.pitch.shift(distance);
    }
}

VxFile VxFile::shifted(int distance) {
    VxFile copy = VxFile(*this);
    copy.shift(distance);
    return copy;
}

void VxFile::removeSilenceSpaceFromBeginning() {
    assert(!pitches.empty());

    int firstPitchStartTickNumber = pitches[0].startTickNumber;
    for (auto& pitch : pitches) {
        pitch.startTickNumber -= firstPitchStartTickNumber;
    }
}

bool VxFile::hasPitchesInMoment(double time) const {
    int tick = timeInSecondsToTicks(time);
    return Contains(pitches, [=] (const VxPitch& pitch) {
        return pitch.containsTick(tick);
    });
}

bool VxFile::hasPitchInMoment(double time, const Pitch &pitch) const {
    int tick = timeInSecondsToTicks(time);
    return Contains(pitches, [=] (const VxPitch& vxPitch) {
        return vxPitch.containsTick(tick) && vxPitch.pitch.getPerfectFrequencyIndex() == pitch.getPerfectFrequencyIndex();
    });
}

double VxFile::getFirstPitchStartTime() const {
    return ticksToSeconds(pitches.front().startTickNumber);
}

VxFile VxFile::withChangedTempo(double tempoFactor) const {
    return VxFile(pitches, distanceInTicksBetweenLastPitchEndAndTrackEnd, (int)round(ticksPerSecond * tempoFactor));
}

VxFile VxFile::cut(double start, double end) {
    std::vector<VxPitch> newPitches;
    int startTick = timeInSecondsToTicks(start);
    int endTick = timeInSecondsToTicks(end);
    iteratePitchesInTimeRange(start, end, [&] (VxPitch vxPitch) {
        if (vxPitch.startTickNumber < startTick) {
            vxPitch.startTickNumber = startTick;
        }

        if(vxPitch.endTickNumber() >= endTick) {
            vxPitch.ticksCount = endTick - vxPitch.startTickNumber;
        }

        newPitches.push_back(vxPitch);
    });

    int distanceInTicksBetweenLastPitchEndAndTrackEnd = this->distanceInTicksBetweenLastPitchEndAndTrackEnd;
    std::max(0, distanceInTicksBetweenLastPitchEndAndTrackEnd - (durationInTicks - endTick));

    return VxFile(std::move(newPitches), distanceInTicksBetweenLastPitchEndAndTrackEnd, ticksPerSecond);
}

int VxFile::getLowestPitchIndex() const {
    return lowestPitchIndex;
}

int VxFile::getHighestPitchIndex() const {
    return highestPitchIndex;
}

const VxPitch& VxFile::getLowestVxPitch() const {
    assert(lowestPitchIndex >= 0);
    return pitches[lowestPitchIndex];
}

const VxPitch& VxFile::getHighestVxPitch() const {
    assert(highestPitchIndex >= 0);
    return pitches[highestPitchIndex];
}

const Pitch& VxFile::getLowestPitch() const {
    return getLowestVxPitch().pitch;
}

const Pitch &VxFile::getHighestPitch() const {
    return getHighestVxPitch().pitch;
}
