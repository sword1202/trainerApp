//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXFILE_H
#define VOCALTRAINER_MVXFILE_H

#include <iostream>
#include "VxFile.h"

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
    std::string recordingData;

    bool readOnlySignature = false;
    int version = 0;

    friend class boost::serialization::access;

    template<typename Archive>
    void load(Archive & ar, const unsigned int version)
    {
        doSerialize(ar, version, readOnlySignature);
        if (version < 1) {
            recording = false;
        }
    }

    template<typename Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        const_cast<MvxFile*>(this)->doSerialize(ar, version, false);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    template<typename Archive>
    void doSerialize(Archive & ar, const unsigned int version, bool readOnlySignature){
        this->version = version;

        if (version < 1) {
            ar & beatsPerMinute;
            ar & vxFile;
            ar & instrumental;
            ar & songTitleUtf8;
            ar & artistNameUtf8;
            ar & score;
            ar & recordingData;
        } else {
            ar & recording;
            ar & songTitleUtf8;
            ar & artistNameUtf8;
            ar & score;

            if (!readOnlySignature) {
                ar & beatsPerMinute;
                ar & vxFile;
                ar & instrumental;
                ar & recordingData;
            }
        }
    }

public:
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

    const std::string &getInstrumental() const;
    std::string &&moveInstrumental();
    void setInstrumental(const std::string &instrumental);

    const std::string &getRecordingData() const;

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
};

BOOST_CLASS_VERSION(MvxFile, 1)


#endif //VOCALTRAINER_MVXFILE_H
