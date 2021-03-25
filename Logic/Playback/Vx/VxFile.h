//
// Created by Semyon Tykhonenko on 1/19/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILE_H
#define VOCALTRAINER_VXFILE_H


#include "VocalPart.h"
#include "VocalTrainerFile.h"
#include "Lyrics.h"
#include "AudioData.h"
#include <sstream>

class VxFile : public VocalTrainerFile {
    VocalPart vocalPart;
    int beatsPerMinute;
public:
    void readFromStream(std::istream &is);
    void parse(const std::string& source);

    // VocalTrainerFile
    const VocalPart &getVocalPart() const override;
    const std::map<double, int> &getRecordingTonalityChanges() const override;
    const Lyrics &getLyrics() const override;
    const AudioData &getInstrumental() const override;
    bool isRecording() const override;
    const std::vector<float> &getRecordedPitchesFrequencies() const override;
    const std::vector<double> &getRecordedPitchesTimes() const override;
    const AudioData &getRecordingData() const override;
    const std::string &getArtistNameUtf8() const override;
    const std::string &getSongTitleUtf8() const override;
    double getBeatsPerMinute() const override;
    const std::vector<short> &getInstrumentalPreviewSamples() const override;

    const Tonality &getOriginalTonality() const override;
};


#endif //VOCALTRAINER_VXFILE_H
