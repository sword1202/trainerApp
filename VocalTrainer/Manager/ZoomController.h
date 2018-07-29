//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_ZOOMCONTROLLER_H
#define VOCALTRAINER_ZOOMCONTROLLER_H

#include "ListenersSet.h"
#include "Pitch.h"

class ZoomController {
    float zoom;
    Pitch firstPitch;
    Pitch lastPitch;
    float verticalScrollPosition;

public:
    CppUtils::ListenersSet<float> zoomChangedListeners;
    CppUtils::ListenersSet<const Pitch&> firstPitchChangedListeners;
    CppUtils::ListenersSet<const Pitch&> lastPitchChangedListeners;
    CppUtils::ListenersSet<float> verticalScrollPositionChangedListeners;
    CppUtils::ListenersSet<> summarizedWorkspaceGridHeightChangedListeners;

    ZoomController();

    float getIntervalWidth() const;
    float getIntervalHeight() const;

    float getZoom() const;
    void setZoom(float zoom);

    float getMinZoom() const;
    float getMaxZoom() const;

    Pitch getFirstPitch() const;
    void setFirstPitch(const Pitch& pitch);

    const Pitch &getLastPitch() const;
    void setLastPitch(const Pitch &lastPitch);
    float getSummarizedWorkspaceGridHeight() const;

    float getVerticalScrollPosition() const;
    void setVerticalScrollPosition(float verticalScrollPosition);
};


#endif //VOCALTRAINER_ZOOMCONTROLLER_H
