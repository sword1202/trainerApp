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
#include <boost/variant.hpp>
#include <map>
#include <boost/serialization/map.hpp>

class MvxFile {
    // signature
    bool recording = false;
    std::string songTitleUtf8;
    std::string artistNameUtf8;
    double score = -1.0;

    // core data
    VocalPart vocalPart;
    std::string instrumental;
    double beatsPerMinute = 0;

    // recording
    std::string recordingData;
    std::vector<double> recordedPitchesTimes;
    std::vector<float> recordedPitchesFrequencies;
    std::map<double, int> recordingTonalityChanges; // seek -> pitchSifting

    std::vector<short> instrumentalPreviewSamples;

    Lyrics lyrics;

    bool readOnlySignature = false;
    int version = 0;

    friend class boost::serialization::access;

    template<typename Archive>
    void load(Archive & ar, const unsigned int version) {
        doSerialize(ar, version, readOnlySignature);
    }

    template<typename Archive>
    void save(Archive & ar, const unsigned int version) const {
        assert(beatsPerMinute > 0 && "beatsPerMinute not set");
        const_cast<MvxFile*>(this)->doSerialize(ar, version, false);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    template<typename Archive>
    void doSerialize(Archive & ar, const unsigned int version, bool readOnlySignature){
        this->version = version;

        ar & recording;
        ar & songTitleUtf8;
        ar & artistNameUtf8;
        ar & score;

        if (!readOnlySignature) {
            ar & beatsPerMinute;
            ar & vocalPart;
            ar & instrumental;
            ar & recordingData;

            if (version >= 2) {
                ar & recordedPitchesTimes;
                ar & recordedPitchesFrequencies;
            }

            if (version >= 3) {
                ar & instrumentalPreviewSamples;
            }

            if (version >= 5) {
                ar & recordingTonalityChanges;
            }

            if (version >= 4) {
                ar & lyrics;
            }
        }
    }

public:
    MvxFile() = default;

    MvxFile(MvxFile&&) = default;
    MvxFile& operator=(MvxFile&&) = default;
    // Preferably use move constructor instead
    MvxFile(const MvxFile& mvxFile) = delete;

    void writeToStream(std::ostream& os) const;
    void writeToFile(const char* outFilePath) const;

    void readFromStream(std::istream &is, bool readOnlySignature = false);
    void readFromFile(const char *filePath, bool readOnlySignature = false);

    const VocalPart &getVocalPart() const;
    void setVocalPart(const VocalPart &vocalPart);

    const std::string &getInstrumental() const;
    void setInstrumental(const std::string &instrumental);

    const std::string &getRecordingData() const;
    std::string& moveRecordingData();
    void setRecordingData(const std::string &recordingData);

    double getBeatsPerMinute() const;
    void setBeatsPerMinute(double beatsPerMinute);

    double getScore() const;
    void setScore(double score);

    const std::string &getSongTitleUtf8() const;
    void setSongTitleUtf8(const std::string &songTitleUtf8);

    const std::string &getArtistNameUtf8() const;
    void setArtistNameUtf8(const std::string &artistNameUtf8);

    void loadInstrumentalFromStream(std::istream& is);
    void loadInstrumentalFromFile(const char* filePath);

    bool isRecording() const;

    const std::vector<double> &getRecordedPitchesTimes() const;
    void setRecordedPitchesTimes(const std::vector<double> &recordedPitchesTimes);
    const std::vector<float> &getRecordedPitchesFrequencies() const;
    void setRecordedPitchesFrequencies(const std::vector<float> &recordedPitchesFrequencies);

    const std::vector<short> &getInstrumentalPreviewSamples() const;
    void setInstrumentalPreviewSamples(const std::vector<short> &instrumentalPreviewSamples);

    void generateInstrumentalPreviewSamplesFromInstrumental();

    const Lyrics &getLyrics() const;
    void setLyrics(const Lyrics &lyrics);

    const std::map<double, int> &getRecordingTonalityChanges() const;
    void setRecordingTonalityChanges(const std::map<double, int> &recordingTonalityChanges);

    std::string convertInstrumentalAndVocalTrackToWav(float vocalVolume = 0.5f) const;
};


/**
 * Version 2: recordedPitchesTimes and recordedPitchesFrequencies are added
 * Version 3: instrumentalPreviewSamples added
 * Version 4: Lyrics added
 * Version 5: recordingTonalityChanges added
 */
BOOST_CLASS_VERSION(MvxFile, 5)


#endif //VOCALTRAINER_MVXFILE_H
