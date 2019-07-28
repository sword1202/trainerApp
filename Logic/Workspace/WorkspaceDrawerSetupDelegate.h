//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACEDRAWERSETUPDELEGATE_H
#define VOCALTRAINER_WORKSPACEDRAWERSETUPDELEGATE_H

#include "Bitmap.h"
#include "Drawer.h"

class WorkspaceDrawerSetupDelegate {
public:
    virtual inline void provideClockFont(int* fontSize, Drawer::FontStyle* style) const {}
    virtual inline void provideYardStickFont(int* fontSize, Drawer::FontStyle* style) const {}
    virtual inline void providePianoFont(int* fontSize, Drawer::FontStyle* style) const {}

    virtual CppUtils::Bitmap createImageForCharacter(char ch, int fontSize, CppUtils::Color color,
            Drawer::FontStyle fontStyle) const = 0;
    /*
     * playHeadTriangle,
     * clock,
     * instrumentalTrackButton,
     * pianoTrackButton
     * */
    virtual CppUtils::Bitmap createImageForName(const char* imageName) const = 0;
    virtual ~WorkspaceDrawerSetupDelegate() = default;
};


#endif //VOCALTRAINER_WORKSPACEDRAWERSETUPDELEGATE_H
