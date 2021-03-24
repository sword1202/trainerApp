//
// Created by Semyon Tikhonenko on 12/16/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_IOSWORKSPACEDRAWERRESOURCESPROVIDER_H
#define VOCALTRAINER_IOSWORKSPACEDRAWERRESOURCESPROVIDER_H

#include <Logic/WorkspaceDrawerResourcesProvider.h>

class iOSWorkspaceDrawerResourcesProvider : public WorkspaceDrawerResourcesProvider {
public:
    CppUtils::Bitmap createImageForCharacter(char ch, int fontSize, CppUtils::Color color, Drawer::FontStyle fontStyle, float scaleFactor) const override;
    CppUtils::Bitmap createImageForName(Image image, int widthInPoints, int heightInPoints, float scaleFactor) const override;
};


#endif //VOCALTRAINER_IOSWORKSPACEDRAWERRESOURCESPROVIDER_H
