#include <assert.h>
#include "VxFile.h"
#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "GetSf2FilePath.h"
#include "WAVFile.h"

static const char *const SILENCE_MARK = "*";

VxFile::VxFile(const std::vector<VxPitch> &pitches, int trackEndSilenceBitsCount, int bitsPerMinute)
        : pitches(pitches), bitsPerMinute(bitsPerMinute), trackEndSilenceBitsCount(trackEndSilenceBitsCount) {
    postInit();
}

VxFile::VxFile(std::istream &is) {
    is >> bitsPerMinute;

    std::string pitchName;
    while (!is.eof()) {
        is >> pitchName;
        if (pitchName == SILENCE_MARK) {
            is >> trackEndSilenceBitsCount;
            break;
        }
        
        VxPitch vxPitch;
        vxPitch.pitch = Pitch(pitchName.data());
        if (!vxPitch.pitch.hasPerfectFrequency()) {
            throw std::runtime_error("Error while parsing pitch with " + pitchName + " name");
        }

        is >> vxPitch.startBitNumber;
        is >> vxPitch.bitsCount;

        pitches.push_back(vxPitch);
    }

    postInit();
}

static inline size_t addSilence(std::vector<char>& pcmData, double duration, int sampleRate) {
    size_t size = static_cast<size_t>(duration * sampleRate * sizeof(short));
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
    double bitDuration = getBitDuration();

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
        addSilence(pcmData, (iter->startBitNumber - silenceStart) * bitDuration, sampleRate);
        tsf_note_on(t, 0, iter->pitch.getSoundFont2Index(), 1.0f);
        silenceStart = iter->startBitNumber + iter->bitsCount;

        // add pitch itself
        size_t currentSize = pcmData.size();
        double duration = bitDuration * iter->bitsCount;
        // resize pcmData to append pitch data
        size_t sizeInBytes = addSilence(pcmData, duration, sampleRate);
        tsf_render_short(t, (short*)(pcmData.data() + currentSize), (int)sizeInBytes / 2, 0);
        
        iter++;
    }

    double endSilenceDuration = trackEndSilenceBitsCount * bitDuration;
    addSilence(pcmData, endSilenceDuration, sampleRate);

    return pcmData;
}

std::vector<char> VxFile::generateWavAudioData() const {
    WavConfig wavConfig;
    std::vector<char> pcmData = generateRawPcmAudioData(wavConfig.sampleRate);
    return WAVFile::addWavHeaderToRawPcmData(pcmData.data(), (int)pcmData.size(), wavConfig);
}

double VxFile::getBitDuration() const {
    return 60.0 / (double) bitsPerMinute;
}

bool VxFile::validate() {
    if (!pitches.empty()) {
        if (pitches[0].startBitNumber < 0) {
            return false;
        }
        
        if (pitches[0].bitsCount < 1) {
            return false;
        }
    }
    
    for (int i = 1; i < pitches.size(); ++i) {
        const VxPitch &vxPitch = pitches[i];
        if (!vxPitch.pitch.hasPerfectFrequency()) {
            return false;
        }
        
        if (vxPitch.bitsCount < 1) {
            return false;
        }

        const VxPitch &prev = pitches[i - 1];
        if (vxPitch.startBitNumber < prev.startBitNumber + prev.bitsCount) {
            return false;
        }
    }

    return true;
}

void VxFile::postInit() {
    assert(trackEndSilenceBitsCount >= 0);
    assert(validate());
    if (!pitches.empty()) {
        const VxPitch &lastPitch = pitches.back();
        durationInBits = lastPitch.startBitNumber + lastPitch.bitsCount + trackEndSilenceBitsCount;
    }
}

double VxFile::getDurationInSeconds() const {
    return getBitDuration() * durationInBits;
}

const std::vector<VxPitch> &VxFile::getPitches() const {
    return pitches;
}

int VxFile::getBitsPerMinute() const {
    return bitsPerMinute;
}

int VxFile::getDurationInBits() const {
    return durationInBits;
}

int VxFile::getTrackEndSilenceBitsCount() const {
    return trackEndSilenceBitsCount;
}

void VxFile::writeToStream(std::ostream &os) const {
    os<<bitsPerMinute<<" ";

    for (const VxPitch& vxPitch : pitches) {
        os<<vxPitch.pitch.getFullName()<<" "<<vxPitch.startBitNumber<<" "<<vxPitch.bitsCount<<" ";
    }

    os<<"* "<<trackEndSilenceBitsCount;
}

VxFile VxFile::fromFilePath(const char *filePath) {
    std::ifstream is(filePath);
    return VxFile(is);
}
