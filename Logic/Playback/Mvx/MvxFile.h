//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXFILE_H
#define VOCALTRAINER_MVXFILE_H

#include <iostream>
#include "VocalPart.h"
#include "StringUtils.h"
#include "Lyrics.h"
#include "StlContainerAudioDataBuffer.h"
#import "VocalTrainerFile.h"
#include "Tonality.h"
#include <boost/variant.hpp>
#include <map>
#import <Logic/TimeSignature.h>
#include "Serializers.h"
#include "TimeSignature.h"
#include "AudioDataBufferSerialization.h"

struct MvxFileHeader {
    bool recording = false;
    std::string songTitleUtf8;
    std::string artistNameUtf8;
    Tonality originalTonality;
    double recordingTempoFactor = 1.0;
    // recording
    std::vector<short> recordingPreviewSamples;

    template <typename Archive>
    void saveOrLoadHeader(Archive& ar, int version) {
        ar(recording);
        ar(songTitleUtf8);
        ar(artistNameUtf8);
        ar(originalTonality);
        ar(recordingTempoFactor);
        ar(recordingPreviewSamples);
    }

    void readFromStream(std::istream& is);
    void readFromFile(const char* filePath);
};

class MvxFile : private MvxFileHeader, public VocalTrainerFile {
    // core data
    VocalPart vocalPart;
    AudioDataBufferConstPtr instrumental = nullptr;
    double beatsPerMinute = 0;
    TimeSignature timeSignature;

    // recording
    AudioDataBufferConstPtr recordingData = nullptr;
    std::vector<double> recordedPitchesTimes;
    std::vector<float> recordedPitchesFrequencies;
    std::vector<short> instrumentalPreviewSamples;
    std::map<double, int> recordingTonalityChanges; // seek -> pitchSifting

    Lyrics lyrics;
public:
    static constexpr int VERSION = 1;
    static constexpr int SERIALIZATION_ID = 12343434;

    template<typename Archive>
    void saveOrLoad(Archive &ar, bool isSave, int version) {
        saveOrLoadHeader(ar, version);

        ar(beatsPerMinute);
        ar(timeSignature);
        ar(vocalPart);

        ar(instrumental);
        ar(recordingData);

        ar(recordedPitchesTimes);
        ar(recordedPitchesFrequencies);
        ar(recordingTonalityChanges);
        ar(lyrics);
        ar(instrumentalPreviewSamples);
    }

    // Preferably use move constructor instead
    MvxFile(const MvxFile &mvxFile) = delete;
    MvxFile(const VocalTrainerFile *file);

    MvxFile() = default;
    MvxFile(MvxFile &&) = default;
    MvxFile &operator=(MvxFile &&) = default;

    void writeToStream(std::ostream &os) const;
    void writeToFile(const char *outFilePath) const;

    void readFromStream(std::istream &is);
    void readFromFile(const char *filePath);

    const VocalPart &getVocalPart() const;
    void setVocalPart(const VocalPart &vocalPart);

    virtual AudioDataBufferConstPtr getInstrumental() const;
    void setInstrumental(const std::string &instrumental);
    void setInstrumental(AudioDataBufferConstPtr instrumental);


    virtual AudioDataBufferConstPtr getRecordingData() const;
    void setRecordingData(const std::string &recordingData);
    void setRecordingData(AudioDataBufferConstPtr recordingData);

    double getBeatsPerMinute() const;
    void setBeatsPerMinute(double beatsPerMinute);

    const std::string &getSongTitleUtf8() const;
    void setSongTitleUtf8(const std::string &songTitleUtf8);
    const std::string &getArtistNameUtf8() const;
    void setArtistNameUtf8(const std::string &artistNameUtf8);

    void loadInstrumentalFromStream(std::istream &is);
    void loadInstrumentalFromFile(const char *filePath);
    void loadLyricsFromStream(std::istream &is);
    void loadLyricsFromFile(const char *filePath);

    bool isRecording() const;
    const std::vector<double> &getRecordedPitchesTimes() const;
    void setRecordedPitchesTimes(const std::vector<double> &recordedPitchesTimes);
    const std::vector<float> &getRecordedPitchesFrequencies() const;
    void setRecordedPitchesFrequencies(const std::vector<float> &recordedPitchesFrequencies);
    const std::map<double, int> &getRecordingTonalityChanges() const;
    void setRecordingTonalityChanges(const std::map<double, int> &recordingTonalityChanges);
    double getRecordingTempoFactor() const override;
    void setRecordingTempoFactor(double tempoFactor);

    const std::vector<short> &getInstrumentalPreviewSamples() const;
    void setInstrumentalPreviewSamples(const std::vector<short> &instrumentalPreviewSamples);
    void generateInstrumentalPreviewSamplesFromInstrumental();

    const std::vector<short> &getRecordingPreviewSamples() const;
    void setRecordingPreviewSamples(const std::vector<short> &recordingPreviewSamples);

    const Lyrics &getLyrics() const;
    void setLyrics(const Lyrics &lyrics);

    const Tonality &getOriginalTonality() const override;
    void setOriginalTonality(const Tonality &originalTonality);

    const TimeSignature &getTimeSignature() const override;
    void setTimeSignature(const TimeSignature &timeSignature);
};


#endif //VOCALTRAINER_MVXFILE_H
