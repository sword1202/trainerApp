//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_NSWORKSPACEDRAWERIMAGESFACTORY_H
#define VOCALTRAINER_NSWORKSPACEDRAWERIMAGESFACTORY_H

#include <Logic/WorkspaceDrawerImagesFactory.h>

class NSWorkspaceDrawerImagesFactory : public WorkspaceDrawerImagesFactory {
public:
    CppUtils::Bitmap createImageForCharacter(char ch, int fontSize, CppUtils::Color color) const override;
    CppUtils::Bitmap createImageForName(const char *imageName) const override;
};


#endif //VOCALTRAINER_NSWORKSPACEDRAWERIMAGESFACTORY_H
