#include <boost/assert.hpp>
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

NoteInterval::NoteInterval(const Pitch &pitch, int startTickNumber, int ticksCount)
        : Interval(startTickNumber, ticksCount), pitch(pitch) {
}

std::ostream& operator<<(std::ostream& os, const NoteInterval& pitch) {
    os<<pitch.pitch<<"["<<pitch.startTickNumber<<","<<pitch.endTickNumber()<<"]";
    return os;
}

VocalPart::VocalPart(std::vector<NoteInterval> &&pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, double ticksPerSecond)
        : notes(std::move(pitches)),
          ticksPerSecond(ticksPerSecond),
          endSilenceDurationInTicks(distanceInTicksBetweenLastPitchEndAndTrackEnd) {
    postInit();
}

VocalPart::VocalPart(const std::vector<NoteInterval> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, double ticksPerSecond)
        : notes(pitches),
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

bool VocalPart::validateNotes() {
    if (!notes.empty()) {
        if (notes[0].startTickNumber < 0) {
            return false;
        }
        
        if (notes[0].ticksCount < 1) {
            return false;
        }
    }
    
    for (int i = 1; i < notes.size(); ++i) {
        const NoteInterval &vxPitch = notes[i];
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
    SortByKey(notes, startTickNumberKeyProvider);
    auto pair = FindMinMaxUsingKeyProvider(notes, [](const NoteInterval& vxPitch) {
        return vxPitch.pitch.getPerfectFrequencyIndex();
    });

    lowestPitchIndex = static_cast<int>(pair.first - notes.begin());
    highestPitchIndex = static_cast<int>(pair.second - notes.begin());

    BOOST_ASSERT(endSilenceDurationInTicks >= 0);
    BOOST_ASSERT(validateNotes());
    if (!notes.empty()) {
        auto lastPitchIter = MaxByKey(notes, [] (const NoteInterval& vxPitch) {
            return vxPitch.endTickNumber();
        });
        durationInTicks = lastPitchIter->endTickNumber() + endSilenceDurationInTicks;
    }
}

double VocalPart::getDurationInSeconds() const {
    return getTickDurationInSeconds() * durationInTicks;
}

const std::vector<NoteInterval> &VocalPart::getNotes() const {
    return notes;
}

double VocalPart::getTicksPerSecond() const {
    return ticksPerSecond;
}

int VocalPart::getDurationInTicks() const {
    return durationInTicks;
}

int VocalPart::getEndSilenceDurationInTicks() const {
    return endSilenceDurationInTicks;
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

int VocalPart::startTickNumberKeyProvider(const NoteInterval &pitch) {
    return pitch.startTickNumber;
}

const NoteInterval &VocalPart::getShortestPitch() const {
    return FindMinValueUsingKeyProvider(notes, [](const NoteInterval& pitch) {
        return pitch.ticksCount;
    });
}

VocalPart::VocalPart() {
}

void VocalPart::setNotes(const std::vector<NoteInterval> &pitches) {
    VocalPart::notes = pitches;
    postInit();
}

bool VocalPart::canBeShifted(int distance) const {
    return notes[lowestPitchIndex].pitch.canBeShifted(distance) &&
            notes[highestPitchIndex].pitch.canBeShifted(distance);
}

void VocalPart::shift(int distance) {
    assert(canBeShifted(distance));
    for (auto& pitch : notes) {
        pitch.pitch.shift(distance);
    }
}

VocalPart VocalPart::shifted(int distance) {
    VocalPart copy = VocalPart(*this);
    copy.shift(distance);
    return copy;
}

void VocalPart::removeSilenceSpaceFromBeginning() {
    assert(!notes.empty());

    int firstPitchStartTickNumber = notes[0].startTickNumber;
    for (auto& pitch : notes) {
        pitch.startTickNumber -= firstPitchStartTickNumber;
    }
}

bool VocalPart::hasPitchesInMoment(double time) const {
    int tick = timeInSecondsToTicks(time);
    return Contains(notes, [=] (const NoteInterval& pitch) {
        return pitch.containsTick(tick);
    });
}

bool VocalPart::hasPitchInMoment(double time, const Pitch &pitch) const {
    int tick = timeInSecondsToTicks(time);
    return Contains(notes, [=] (const NoteInterval& vxPitch) {
        return vxPitch.containsTick(tick) && vxPitch.pitch.getPerfectFrequencyIndex() == pitch.getPerfectFrequencyIndex();
    });
}

double VocalPart::getFirstPitchStartTime() const {
    return ticksToSeconds(notes.front().startTickNumber);
}

VocalPart VocalPart::withChangedTempo(double tempoFactor) const {
    return VocalPart(notes, endSilenceDurationInTicks, (int)round(ticksPerSecond * tempoFactor));
}

VocalPart VocalPart::cutOrExpand(double start, double end) {
    std::vector<NoteInterval> newPitches;
    int startTick = timeInSecondsToTicks(start);
    int endTick = timeInSecondsToTicks(end);
    if (startTick > 0 || endTick < durationInTicks - endSilenceDurationInTicks) {
        iteratePitchesInTimeRange(start, end, [&] (NoteInterval vxPitch) {
            if (vxPitch.startTickNumber < startTick) {
                vxPitch.startTickNumber = startTick;
            }

            if(vxPitch.endTickNumber() >= endTick) {
                vxPitch.ticksCount = endTick - vxPitch.startTickNumber;
            }

            newPitches.push_back(vxPitch);
        });
    } else {
        newPitches = notes;
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

const NoteInterval& VocalPart::getLowestNote() const {
    assert(lowestPitchIndex >= 0);
    return notes[lowestPitchIndex];
}

const NoteInterval& VocalPart::getHighestNote() const {
    assert(highestPitchIndex >= 0);
    return notes[highestPitchIndex];
}

const Pitch& VocalPart::getLowestPitch() const {
    return getLowestNote().pitch;
}

const Pitch &VocalPart::getHighestPitch() const {
    return getHighestNote().pitch;
}

bool VocalPart::isEmpty() const {
    return durationInTicks == 0;
}

double VocalPart::getPitchDuration(int index) const {
    assert(index < notes.size());
    return ticksToSeconds(notes[index].ticksCount);
}
