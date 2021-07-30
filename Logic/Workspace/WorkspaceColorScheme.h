//
// Created by Semyon Tikhonenko on 5/1/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACECOLORSCHEME_H
#define VOCALTRAINER_WORKSPACECOLORSCHEME_H

#include "Drawer.h"

struct WorkspaceColorScheme {
    typedef Drawer::Color Color;
    Color gridColor;
    Color accentGridColor;
    Color pitchGraphColor;
    Color pianoBorderColor;
    Color pitchColor;
    Color reachedPitchColor;
    Color missedPitchColor;
    Color pianoSharpPitchColor;
    Color pianoSelectedPitchColor;
    Color borderLineColor;
    Color boundsColor;
    Color playHeadColor;
    Color instrumentalTrackColor;
    Color pianoTrackColor;
    Color pianoTrackShadowColor;
    Color pianoTrackPitchesColor;
    Color endingColor;

    WorkspaceColorScheme();
};


#endif //VOCALTRAINER_WORKSPACECOLORSCHEME_H
