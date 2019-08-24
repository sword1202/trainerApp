//
// Created by Semyon Tykhonenko on 8/24/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_BOUNDSSELECTIONDELEGATE_H
#define VOCALTRAINER_BOUNDSSELECTIONDELEGATE_H

#include "PlaybackBounds.h"

class BoundsSelectionDelegate {
public:
    virtual void onPlaybackBoundsChangedByUserEvent(const PlaybackBounds& newBounds) = 0;
    virtual ~BoundsSelectionDelegate() = default;
};

#endif //VOCALTRAINER_BOUNDSSELECTIONDELEGATE_H
