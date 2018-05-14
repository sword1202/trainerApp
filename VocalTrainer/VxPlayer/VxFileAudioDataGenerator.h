//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
#define VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H

#include "StlDebugUtils.h"
#include "VxFile.h"
#include "tsf.h"
#include "PitchRenderer.h"
#include <set>
#include <boost/icl/interval_set.hpp>
#include <mutex>

struct VxFileAudioDataGeneratorConfig {
    int sampleRate = 44100;
    int outBufferSize = 512;
};


// renderNextPitchIfPossible and readNextSamplesBatch should be called from 2 different threads, other methods except
// setVxFile can be called from any thread
class VxFileAudioDataGenerator {
    PitchRenderer* renderer;
    VxFile vxFile;
    std::vector<short> pcmData;
    std::vector<short> fullyInitializedPcmData;
    std::vector<char> pcmHasDataFlag;
    boost::icl::interval_set<int> fullyInitializedDataIntervals;

    std::set<int> renderedPitchesIndexes;
    int outBufferSize;
    int seek = 0;
    int sampleRate;
    int renderedPitchesCount = 0;
    mutable std::mutex bufferReadingMutex;
    mutable std::mutex seekMutex;

    bool isFullyInitialized(int begin, int end) const;
    int getNextPitchToRenderIndex() const;
    void renderPitch(const Pitch &pitch, int begin, int length);

    void clearAllData();
public:
    VxFileAudioDataGenerator(PitchRenderer *renderer, const VxFile &vxFile,
            const VxFileAudioDataGeneratorConfig &config);
    VxFileAudioDataGenerator(PitchRenderer *renderer, const VxFile &vxFile);
    VxFileAudioDataGenerator(const VxFile &vxFile, const VxFileAudioDataGeneratorConfig &config);
    VxFileAudioDataGenerator(const VxFile &vxFile);

    virtual ~VxFileAudioDataGenerator();

    bool renderNextPitchIfPossible();
    // returns size = -1 if no data available and you should wait for some data rendered.
    int readNextSamplesBatch(short *intoBuffer);

    int getTotalSamplesCount() const;
    double getDurationInSeconds() const ;
    int getOutBufferSize() const;
    int getSampleRate() const;

    void renderAllData();
    void setSeek(int seek);

    int getSeek() const;

    // should be called from renderNextPitchIfPossible thread
    void setVxFile(const VxFile& vxFile);

    const VxFile &getVxFile() const;
};


#endif //VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
