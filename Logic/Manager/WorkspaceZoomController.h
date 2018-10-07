//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_ZOOMCONTROLLER_H
#define VOCALTRAINER_ZOOMCONTROLLER_H

#include "ListenersSet.h"
#include "Pitch.h"
#include "WorkspaceController.h"

class WorkspaceZoomController {
    std::atomic<float> zoom;
    std::atomic_int firstPitchIndex;
    std::atomic_int lastPitchIndex;
    float verticalScrollPosition = 0;
    std::atomic<float> workspaceGridHeight;
public:
    CppUtils::ListenersSet<float> zoomChangedListeners;
    CppUtils::ListenersSet<const Pitch&> firstPitchChangedListeners;
    CppUtils::ListenersSet<const Pitch&> lastPitchChangedListeners;
    CppUtils::ListenersSet<float> verticalScrollPositionChangedListeners;
    CppUtils::ListenersSet<> summarizedWorkspaceGridHeightChangedListeners;

    WorkspaceZoomController();

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
    float getPageSize() const;

    virtual void onWorkspaceWidgetHeightChanged(float height);

    float getVerticalScrollPosition() const;
    void setVerticalScrollPosition(float verticalScrollPosition);
};


#endif //VOCALTRAINER_ZOOMCONTROLLER_H
