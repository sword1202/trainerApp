//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_ZOOMCONTROLLER_H
#define VOCALTRAINER_ZOOMCONTROLLER_H

#include "ListenersSet.h"

class ZoomController {
    float zoom;
    int firstPitchPerfectFrequencyIndex;

    CppUtils::ListenersSet<float> zoomChangedListeners;
    CppUtils::ListenersSet<int> firstPitchPerfectFrequencyIndexChangedListeners;
public:
    ZoomController();

    float getIntervalWidth() const;
    float getIntervalHeight() const;

    float getZoom() const;
    void setZoom(float zoom);

    float getMinZoom() const;
    float getMaxZoom() const;

    int getFirstPitchPerfectFrequencyIndex() const;
    void setFirstPitchPerfectFrequencyIndex(int firstPitchPerfectFrequencyIndex);

    int addFirstPitchPerfectFrequencyIndexChangedListener(const CppUtils::ListenersSet<int>::Listener& listener);
    void removeFirstPitchPerfectFrequencyIndexChangedListener(int key);
    int addZoomChangedListener(const CppUtils::ListenersSet<float>::Listener& listener);
    void removeZoomChangedListener(int key);
};


#endif //VOCALTRAINER_ZOOMCONTROLLER_H
