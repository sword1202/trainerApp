//
// Created by Semyon Tikhonenko on 5/1/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACEDRAWERCOLORSCHEME_H
#define VOCALTRAINER_WORKSPACEDRAWERCOLORSCHEME_H

#include "Drawer.h"

struct WorkspaceDrawerColorScheme {
    typedef Drawer::Color Color;
    Color gridColor;
    Color accentGridColor;
    Color pitchGraphColor;
    Color pitchColor;
    Color borderLineColor;
    Color boundsColor;
    Color playHeadColor;
    Color instrumentalTrackColor;
    Color pianoTrackColor;
    Color pianoTrackShadowColor;
    Color pianoTrackPitchesColor;
    Color endingColor;

    static const WorkspaceDrawerColorScheme* getDefault();
};


#endif //VOCALTRAINER_WORKSPACEDRAWERCOLORSCHEME_H
