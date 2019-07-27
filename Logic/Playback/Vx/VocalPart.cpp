#include <boost/assert.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <utf8.h>

#include "config.h"
#include "VocalPart.h"
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

VocalPart::VocalPart(std::vector<VxPitch> &&pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond)
        : pitches(std::move(pitches)),
          ticksPerSecond(ticksPerSecond),
          endSilenceDurationInTicks(distanceInTicksBetweenLastPitchEndAndTrackEnd) {
    postInit();
}

VocalPart::VocalPart(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond)
        : pitches(pitches),
          ticksPerSecond(ticksPerSecond),
          endSilenceDurationInTicks(distanceInTicksBetweenLastPitchEndAndTrackEnd)
{
    postInit();
}

static inline size_t addSilence(std::vector<char>& pcmData, double duration, int sampleRate) {
    size_t size = static_cast<size_t>((int)round(duration * sampleRate) * sizeof(short));
    pcmData.resize(pcmData.size() + size);
    return size;
}

double VocalPart::getTickDurationInSeconds() const {
    return 1.0 / (double) ticksPerSecond;
}

bool VocalPart::validatePitches() {
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

void VocalPart::postInit() {
    SortByKey(pitches, startTickNumberKeyProvider);
    auto pair = FindMinMaxUsingKeyProvider(pitches, [](const VxPitch& vxPitch) {
        return vxPitch.pitch.getPerfectFrequencyIndex();
    });

    lowestPitchIndex = pair.first - pitches.begin();
    highestPitchIndex = pair.second - pitches.begin();

    BOOST_ASSERT(endSilenceDurationInTicks >= 0);
    BOOST_ASSERT(validatePitches());
    if (!pitches.empty()) {
        auto lastPitchIter = MaxByKey(pitches, [] (const VxPitch& vxPitch) {
            return vxPitch.endTickNumber();
        });
        durationInTicks = lastPitchIter->endTickNumber() + endSilenceDurationInTicks;
    }
}

double VocalPart::getDurationInSeconds() const {
    return getTickDurationInSeconds() * durationInTicks;
}

const std::vector<VxPitch> &VocalPart::getPitches() const {
    return pitches;
}

int VocalPart::getTicksPerSecond() const {
    return ticksPerSecond;
}

int VocalPart::getDurationInTicks() const {
    return durationInTicks;
}

int VocalPart::getEndSilenceDurationInTicks() const {
    return endSilenceDurationInTicks;
}

void VocalPart::writeToStream(std::ostream &os) const {
    boost::archive::text_oarchive ar(os);
    ar << *this;
}

VocalPart::VocalPart(std::istream &is) {
    boost::archive::text_iarchive ar(is);
    ar >> *this;
    postInit();
}

VocalPart VocalPart::fromFilePath(const char *filePath) {
    std::ifstream is(filePath);
    return VocalPart(is);
}

int VocalPart::timeInSecondsToTicks(double timeInSeconds)const {
    return static_cast<int>(round(timeInSeconds / getTickDurationInSeconds()));
}

double VocalPart::ticksToSeconds(int ticks) const {
    return ticks * getTickDurationInSeconds();
}

int VocalPart::samplesCountFromTicks(int ticks, int sampleRate) const {
    return (int) round(ticksToSeconds(ticks) * sampleRate);
}

int VocalPart::ticksFromSamplesCount(int samplesCount, int sampleRate) const {
    return timeInSecondsToTicks(AudioUtils::GetSampleTimeInSeconds(samplesCount, sampleRate));
}

int VocalPart::startTickNumberKeyProvider(const VxPitch &pitch) {
    return pitch.startTickNumber;
}

const VxPitch &VocalPart::getShortestPitch() const {
    return FindMinValueUsingKeyProvider(pitches, [](const VxPitch& pitch) {
        return pitch.ticksCount;
    });
}

VocalPart::VocalPart() {

}

void VocalPart::setPitches(const std::vector<VxPitch> &pitches) {
    VocalPart::pitches = pitches;
    postInit();
}

bool VocalPart::canBeShifted(int distance) const {
    return pitches[lowestPitchIndex].pitch.canBeShifted(distance) &&
            pitches[highestPitchIndex].pitch.canBeShifted(distance);
}

void VocalPart::shift(int distance) {
    assert(canBeShifted(distance));
    for (auto& pitch : pitches) {
        pitch.pitch.shift(distance);
    }
}

VocalPart VocalPart::shifted(int distance) {
    VocalPart copy = VocalPart(*this);
    copy.shift(distance);
    return copy;
}

void VocalPart::removeSilenceSpaceFromBeginning() {
    assert(!pitches.empty());

    int firstPitchStartTickNumber = pitches[0].startTickNumber;
    for (auto& pitch : pitches) {
        pitch.startTickNumber -= firstPitchStartTickNumber;
    }
}

bool VocalPart::hasPitchesInMoment(double time) const {
    int tick = timeInSecondsToTicks(time);
    return Contains(pitches, [=] (const VxPitch& pitch) {
        return pitch.containsTick(tick);
    });
}

bool VocalPart::hasPitchInMoment(double time, const Pitch &pitch) const {
    int tick = timeInSecondsToTicks(time);
    return Contains(pitches, [=] (const VxPitch& vxPitch) {
        return vxPitch.containsTick(tick) && vxPitch.pitch.getPerfectFrequencyIndex() == pitch.getPerfectFrequencyIndex();
    });
}

double VocalPart::getFirstPitchStartTime() const {
    return ticksToSeconds(pitches.front().startTickNumber);
}

VocalPart VocalPart::withChangedTempo(double tempoFactor) const {
    return VocalPart(pitches, endSilenceDurationInTicks, (int)round(ticksPerSecond * tempoFactor));
}

VocalPart VocalPart::cutOrExpand(double start, double end) {
    std::vector<VxPitch> newPitches;
    int startTick = timeInSecondsToTicks(start);
    int endTick = timeInSecondsToTicks(end);
    if (startTick > 0 || endTick < durationInTicks - endSilenceDurationInTicks) {
        iteratePitchesInTimeRange(start, end, [&] (VxPitch vxPitch) {
            if (vxPitch.startTickNumber < startTick) {
                vxPitch.startTickNumber = startTick;
            }

            if(vxPitch.endTickNumber() >= endTick) {
                vxPitch.ticksCount = endTick - vxPitch.startTickNumber;
            }

            newPitches.push_back(vxPitch);
        });
    } else {
        newPitches = pitches;
    }

    int distanceInTicksBetweenLastPitchEndAndTrackEnd = std::max(0,
            this->endSilenceDurationInTicks - (durationInTicks - endTick));

    return VocalPart(std::move(newPitches), distanceInTicksBetweenLastPitchEndAndTrackEnd, ticksPerSecond);
}

int VocalPart::getLowestPitchIndex() const {
    return lowestPitchIndex;
}

int VocalPart::getHighestPitchIndex() const {
    return highestPitchIndex;
}

const VxPitch& VocalPart::getLowestVxPitch() const {
    assert(lowestPitchIndex >= 0);
    return pitches[lowestPitchIndex];
}

const VxPitch& VocalPart::getHighestVxPitch() const {
    assert(highestPitchIndex >= 0);
    return pitches[highestPitchIndex];
}

const Pitch& VocalPart::getLowestPitch() const {
    return getLowestVxPitch().pitch;
}

const Pitch &VocalPart::getHighestPitch() const {
    return getHighestVxPitch().pitch;
}

bool VocalPart::isEmpty() const {
    return durationInTicks == 0;
}
