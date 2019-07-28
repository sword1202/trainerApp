//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACEDRAWERIMAGESFACTORY_H
#define VOCALTRAINER_WORKSPACEDRAWERIMAGESFACTORY_H

#include "Bitmap.h"

class WorkspaceDrawerImagesFactory {
public:
    virtual CppUtils::Bitmap createImageForCharacter(char ch, int fontSize, CppUtils::Color color) const = 0;
    /*
     * playHeadTriangle,
     * clock,
     * instrumentalTrackButton,
     * pianoTrackButton
     * */
    virtual CppUtils::Bitmap createImageForName(const char* imageName) const = 0;
    virtual ~WorkspaceDrawerImagesFactory() = default;
};


#endif //VOCALTRAINER_WORKSPACEDRAWERIMAGESFACTORY_H
