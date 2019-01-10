//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACECONTROLLER_H
#define VOCALTRAINER_WORKSPACECONTROLLER_H


#include "PitchesCollection.h"
#include "PlayingPitchSequence.h"
#include "PlaybackBounds.h"
#include "VxFile.h"

class WorkspaceController {
public:
    virtual void setIntervalWidth(float intervalWidth) = 0;
    virtual void setIntervalHeight(float intervalHeight) = 0;
    virtual void setPitchesCollection(const PitchesCollection *pitchesCollection) = 0;
    virtual void setIntervalsPerSecond(double intervalsPerSecond) = 0;
    virtual bool isRunning() const = 0;
    virtual void setRunning(bool value) = 0;
    virtual double getIntervalsPerSecond() const = 0;
    virtual void setVerticalOffset(float verticalOffset) = 0;
    virtual void setHorizontalOffset(float horizontalOffset) = 0;
    virtual float getHorizontalOffset() const = 0;
    virtual void setVxFile(const VxFile* vxFile) = 0;
    virtual void setFirstVisiblePitch(const Pitch &firstPitch) = 0;
    virtual void update() = 0;
    virtual void setDetectedPitch(const Pitch &detectedPitch) = 0;
    virtual void setPitchSequence(PlayingPitchSequence *pitchSequence) = 0;
    virtual void setSummarizedGridHeight(float summarizedGridHeight) = 0;
    virtual void setVerticalScrollPosition(float verticalScrollPosition) = 0;
    virtual void setPlaybackBounds(const PlaybackBounds &playbackBounds) = 0;
    virtual float getSeekFromXPositionOnWorkspace(float x) = 0;
    virtual float getWorkspaceSeek() const = 0;
    virtual float getPlayHeadXPosition(int playHeadIndex) = 0;
    virtual void setRecording(bool recording) = 0;
    virtual void setInstrumentalTrackSamples(const std::vector<short> &instrumentalTrackSamples) = 0;
    virtual void setDrawTracks(bool value) = 0;
    virtual ~WorkspaceController() = default;
};


#endif //VOCALTRAINER_WORKSPACECONTROLLER_H
