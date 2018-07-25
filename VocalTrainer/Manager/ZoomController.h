//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_ZOOMCONTROLLER_H
#define VOCALTRAINER_ZOOMCONTROLLER_H

#include "ListenersSet.h"
#include "Pitch.h"

class ZoomController {
    std::atomic<float> zoom;
    Pitch firstPitch;

    CppUtils::ListenersSet<float> zoomChangedListeners;
    CppUtils::ListenersSet<const Pitch&> firstPitchChangedListeners;
public:
    ZoomController();

    float getIntervalWidth() const;
    float getIntervalHeight() const;

    float getZoom() const;
    void setZoom(float zoom);

    float getMinZoom() const;
    float getMaxZoom() const;

    Pitch getFirstPitch() const;
    void setFirstPitch(const Pitch& pitch);

    int addFirstPitchChangedListener(const CppUtils::ListenersSet<const Pitch&>::Listener &listener);
    void removeFirstPitchChangedListener(int key);
    int addZoomChangedListener(const CppUtils::ListenersSet<float>::Listener& listener);
    void removeZoomChangedListener(int key);
};


#endif //VOCALTRAINER_ZOOMCONTROLLER_H
