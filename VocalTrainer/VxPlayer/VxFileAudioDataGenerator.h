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
#include <unordered_set>

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
    std::vector<int> summarizedPcmData;
    std::vector<short> divisionFactor;
    std::vector<short> publishedPcmData;
    boost::icl::interval_set<int> publishedDataIntervals;

    std::set<int> renderedPitchesIndexes;
    std::unordered_set<int> publishedPitchesIndexes;

    int outBufferSize;
    int seek = 0;
    int sampleRate;
    mutable std::mutex bufferReadingMutex;
    mutable std::mutex seekMutex;

    bool isPublished(int begin, int end) const;
    int getNextPitchToRenderIndex() const;
    void renderPitch(const Pitch &pitch, int begin, int length);

    void clearAllData();

    void resetPublishedDataIntervals();

    void publishPitchIfFullyRendered(int index);
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

    int getFullyFilledPcmDataSize() const;
};


#endif //VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
