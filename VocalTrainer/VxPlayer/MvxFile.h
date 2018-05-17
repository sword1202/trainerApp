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
    double beatsPerMinute;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & beatsPerMinute;
        ar & vxFile;
        ar & instrumental;
    }

public:

    MvxFile();
    MvxFile(VxFile &&vxFile, std::string &&instrumental, double beatsPerMinute);
    MvxFile(const VxFile &vxFile, std::istream& instrumentalStream, double beatsPerMinute);
    MvxFile(const VxFile &vxFile, const char* instrumentalFile, double beatsPerMinute);

    MvxFile(MvxFile&& mvxFile) = default;
    // Preferably use move constructor instead
    MvxFile(const MvxFile& mvxFile) = delete;

    void writeToStream(std::ostream& os);
    void writeToFile(const char* outFilePath);

    static MvxFile readFromStream(std::istream& is);
    static MvxFile readFromFile(const char* outFilePath);

    const VxFile &getVxFile() const;
    VxFile &getVxFile();

    const std::string &getInstrumental() const;
    std::string &getInstrumental();

    double getBeatsPerMinute() const;
    void setBeatsPerMinute(double beatsPerMinute);
    void loadInstrumentalFromStream(std::istream& is);
    void loadInstrumentalFromFile(const char* filePath);
};


#endif //VOCALTRAINER_MVXFILE_H
