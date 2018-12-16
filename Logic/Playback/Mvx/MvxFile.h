//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXFILE_H
#define VOCALTRAINER_MVXFILE_H

#include <iostream>
#include "VxFile.h"

class MvxFile {
    VxFile vxFile;
    std::string instrumental;
    double beatsPerMinute = 0;
    std::string songTitleUtf8;
    std::string artistNameUtf8;
    double score = -1.0;
    std::string recording;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & beatsPerMinute;
        ar & vxFile;
        ar & instrumental;
        ar & songTitleUtf8;
        ar & artistNameUtf8;
        ar & score;
        ar & recording;
    }

public:
    MvxFile() = default;

    MvxFile(MvxFile&& mvxFile) = default;
    // Preferably use move constructor instead
    MvxFile(const MvxFile& mvxFile) = delete;

    void writeToStream(std::ostream& os);
    void writeToFile(const char* outFilePath);

    static MvxFile readFromStream(std::istream& is);
    static MvxFile readFromFile(const char* filePath);

    const VxFile &getVxFile() const;
    VxFile &getVxFile();

    const std::string &getInstrumental() const;
    std::string &&moveInstrumental();
    void setInstrumental(const std::string &instrumental);

    const std::string &getRecording() const;

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
};


#endif //VOCALTRAINER_MVXFILE_H
