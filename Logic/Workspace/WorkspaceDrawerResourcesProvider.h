//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACEDRAWERRESOURCESPROVIDER_H
#define VOCALTRAINER_WORKSPACEDRAWERRESOURCESPROVIDER_H

#include "Bitmap.h"
#include "Drawer.h"
#include "EnumMap.h"

class WorkspaceDrawerResourcesProvider {
public:
    enum Image {
        playHeadTriangle = 0, clock, instrumentalTrackButton, pianoTrackButton
    };

    static inline const CppUtils::EnumMap<Image, std::string> imageNames = {
            "playHeadTriangle", "clock", "instrumentalTrackButton", "pianoTrackButton"
    };

    virtual inline void provideClockFont(int* fontSize, Drawer::FontStyle* style) const {}
    virtual inline void provideYardStickFont(int* fontSize, Drawer::FontStyle* style) const {}
    virtual inline void providePianoFont(int* fontSize, Drawer::FontStyle* style) const {}

    virtual CppUtils::Bitmap createImageForCharacter(char ch, int fontSize, CppUtils::Color color,
            Drawer::FontStyle fontStyle, float scaleFactor) const = 0;

    virtual CppUtils::Bitmap createImageForName(Image image, int widthInPoints, int heightInPoints, float scaleFactor) const = 0;
    virtual ~WorkspaceDrawerResourcesProvider() = default;
};


#endif //VOCALTRAINER_WORKSPACEDRAWERRESOURCESPROVIDER_H
