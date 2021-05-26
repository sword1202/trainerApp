//
// Created by Semyon Tykhonenko on 1/20/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERFILE_H
#define VOCALTRAINER_VOCALTRAINERFILE_H


#include "VocalPart.h"
#include "Lyrics.h"
#include "AudioDataBuffer.h"
#include "Tonality.h"
#include <map>
#include <memory>

class VocalTrainerFile {
public:
    // This method dynamically allocates a new file, you are responsible to delete it, when not needed
    static VocalTrainerFile* read(std::istream& is);

    virtual const VocalPart &getVocalPart() const = 0;
    virtual const std::map<double, int> &getRecordingTonalityChanges() const = 0;
    virtual const Lyrics &getLyrics() const = 0;
    virtual AudioDataBufferConstPtr getInstrumental() const = 0;
    virtual bool isRecording() const = 0;
    virtual const std::vector<float>& getRecordedPitchesFrequencies() const = 0;
    virtual const std::vector<double> &getRecordedPitchesTimes() const = 0;
    virtual AudioDataBufferConstPtr getRecordingData() const = 0;
    virtual const std::string &getArtistNameUtf8() const = 0;
    virtual const std::string &getSongTitleUtf8() const = 0;
    virtual double getBeatsPerMinute() const = 0;
    virtual const std::vector<short> &getInstrumentalPreviewSamples() const = 0;
    virtual const Tonality& getOriginalTonality() const = 0;
    inline virtual double getRecordingTempoFactor() const {
        return 1.0;
    }

    virtual ~VocalTrainerFile() = default;
};


#endif //VOCALTRAINER_VOCALTRAINERFILE_H
