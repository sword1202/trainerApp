#include <assert.h>
#include "VxFile.h"
#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "GetSf2FilePath.h"
#include "WAVFile.h"
#include "Algorithms.h"
#include "Strings.h"
#include <utf8.h>

static constexpr char SILENCE_MARK[] = "*";
static constexpr char LYRICS_START[] = "lyricsStart*";
static constexpr char LYRICS_END[] = " lyricsEnd*";
static constexpr char NO_LYRICS[] = "noLyrics*";

using namespace CppUtils;

VxFile::VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int bitsPerMinute)
        : pitches(pitches), ticksPerMinute(bitsPerMinute),
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

    is >> ticksPerMinute;

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
    assert(validateLyrics());
}

static inline size_t addSilence(std::vector<char>& pcmData, double duration, int sampleRate) {
    size_t size = static_cast<size_t>((int)round(duration * sampleRate) * sizeof(short));
    pcmData.resize(pcmData.size() + size);
    return size;
}

// make sure tsf is properly destroyed
struct TsfHolder {
    tsf* t;

    TsfHolder(int sampleRate) {
        t = tsf_load_filename(GetSf2FilePath().data());
        tsf_set_output(t, TSF_MONO, sampleRate, 0);
    }

    ~TsfHolder() {
        tsf_close(t);
    }
};

std::vector<char> VxFile::generateRawPcmAudioData(int sampleRate) const {
    double bitDuration = getTickDuration();

    TsfHolder tsfHolder(sampleRate);
    tsf* t = tsfHolder.t;

    int size = static_cast<int>(getDurationInSeconds() * sampleRate);
    
    std::vector<char> pcmData;
    // make sure there is no buffer overflow
    pcmData.reserve(size + 10u);

    auto iter = pitches.begin();
    auto end = pitches.end();
    if (iter == end) {
        return pcmData;
    }

    int silenceStart = 0;
    while (iter != end) {
        // add silence between pitches
        addSilence(pcmData, (iter->startTickNumber - silenceStart) * bitDuration, sampleRate);
        tsf_note_on(t, 0, iter->pitch.getSoundFont2Index(), 0.5f);
        silenceStart = iter->startTickNumber + iter->ticksCount;

        // add pitch itself
        size_t currentSize = pcmData.size();
        double duration = bitDuration * iter->ticksCount;
        // resize pcmData to append pitch data
        size_t sizeInBytes = addSilence(pcmData, duration, sampleRate);
        tsf_render_short(t, (short*)(pcmData.data() + currentSize), (int)sizeInBytes / 2, 0);
        
        iter++;
    }

    double endSilenceDuration = distanceInTicksBetweenLastPitchEndAndTrackEnd * bitDuration;
    addSilence(pcmData, endSilenceDuration, sampleRate);

    return pcmData;
}

std::vector<char> VxFile::generateWavAudioData() const {
    WavConfig wavConfig;
    std::vector<char> pcmData = generateRawPcmAudioData(wavConfig.sampleRate);
    return WAVFile::addWavHeaderToRawPcmData(pcmData.data(), (int)pcmData.size(), wavConfig);
}

double VxFile::getTickDuration() const {
    return 60.0 / (double) ticksPerMinute;
}

bool VxFile::validate() {
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

        const VxPitch &prev = pitches[i - 1];
        if (vxPitch.startTickNumber < prev.startTickNumber + prev.ticksCount) {
            return false;
        }
    }

    return true;
}

void VxFile::postInit() {
    assert(distanceInTicksBetweenLastPitchEndAndTrackEnd >= 0);
    assert(validate());
    if (!pitches.empty()) {
        const VxPitch &lastPitch = pitches.back();
        durationInTicks = lastPitch.startTickNumber + lastPitch.ticksCount + distanceInTicksBetweenLastPitchEndAndTrackEnd;
    }
}

double VxFile::getDurationInSeconds() const {
    return getTickDuration() * durationInTicks;
}

const std::vector<VxPitch> &VxFile::getPitches() const {
    return pitches;
}

int VxFile::getTicksPerMinute() const {
    return ticksPerMinute;
}

int VxFile::getDurationInTicks() const {
    return durationInTicks;
}

int VxFile::getDistanceInTicksBetweenLastPitchEndAndTrackEnd() const {
    return distanceInTicksBetweenLastPitchEndAndTrackEnd;
}

void VxFile::writeToStream(std::ostream &os) const {
    os<<ticksPerMinute<<" ";

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
    assert(validateLyrics());
}

bool VxFile::validateLyrics() {
    if (!lyrics.empty()) {
        if (lyrics[0].startTimestampInMilliseconds < 0) {
            return false;
        }

        if (lyrics[0].endTimestampInMilliseconds < 1) {
            return false;
        }

        if (lyrics[0].endTimestampInMilliseconds < lyrics[0].startTimestampInMilliseconds) {
            return false;
        }
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
