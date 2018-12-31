//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXFILE_H
#define VOCALTRAINER_MVXFILE_H

#include <iostream>
#include "VxFile.h"
#include "StringUtils.h"
#include <boost/variant.hpp>

class MvxFile {
    // signature
    bool recording = false;
    std::string songTitleUtf8;
    std::string artistNameUtf8;
    double score = -1.0;

    // core data
    VxFile vxFile;
    std::string instrumental;
    double beatsPerMinute = 0;

    // recording
    std::string recordingData;
    std::vector<double> recordedPitchesTimes;
    std::vector<float> recordedPitchesFrequencies;

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
            ar & vxFile;
            ar & instrumental;
            ar & recordingData;

            if (version >= 2) {
                ar & recordedPitchesTimes;
                ar & recordedPitchesFrequencies;
            }
        }
    }

public:
    constexpr static int DEFAULT_PREVIEW_BATCH_SIZE = 1000;

    MvxFile() = default;

    MvxFile(MvxFile&&) = default;
    MvxFile& operator=(MvxFile&&) = default;
    // Preferably use move constructor instead
    MvxFile(const MvxFile& mvxFile) = delete;

    void writeToStream(std::ostream& os);
    void writeToFile(const char* outFilePath);

    static MvxFile readFromStream(std::istream& is, bool readOnlySignature = false);
    static MvxFile readFromFile(const char* filePath, bool readOnlySignature = false);

    const VxFile &getVxFile() const;
    VxFile &getVxFile();
    void setVxFile(const VxFile &vxFile);

    const std::string &getInstrumental() const;
    std::string &moveInstrumental();
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

    std::vector<double> &moveRecordedPitchesTimes();
    void setRecordedPitchesTimes(const std::vector<double> &recordedPitchesTimes);
    std::vector<float> &moveRecordedPitchesFrequencies();
    void setRecordedPitchesFrequencies(const std::vector<float> &recordedPitchesFrequencies);

    static std::vector<int16_t> generatePreviewSamplesFromRawPcm(const std::string& rawPcm,
            int batchSize = DEFAULT_PREVIEW_BATCH_SIZE);
};


/**
 * Version 2: recordedPitchesTimes and recordedPitchesFrequencies are added
 */
BOOST_CLASS_VERSION(MvxFile, 2)


#endif //VOCALTRAINER_MVXFILE_H
