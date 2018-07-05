//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACECONTROLLER_H
#define VOCALTRAINER_WORKSPACECONTROLLER_H


#import "PitchesCollector.h"

class WorkspaceController {
public:
    virtual void setIntervalWidth(float intervalWidth) = 0;
    virtual void setIntervalHeight(float intervalHeight) = 0;
    virtual void setPitchesCollector(PitchesCollector *pitchesCollector) = 0;
    virtual void setIntervalsPerSecond(double intervalsPerSecond) = 0;
    virtual void setVerticalOffset(float verticalOffset) = 0;
    virtual void setHorizontalOffset(float horizontalOffset) = 0;
    virtual void setVxFile(const VxFile* vxFile) = 0;
    virtual void setFirstPitchPerfectFrequencyIndex(int firstPitchPerfectFrequencyIndex) = 0;
    virtual ~WorkspaceController() = default;
};


#endif //VOCALTRAINER_WORKSPACECONTROLLER_H
