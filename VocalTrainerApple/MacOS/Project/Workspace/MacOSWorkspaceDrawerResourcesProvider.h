//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_MACOSWORKSPACEDRAWERRESOURCESPROVIDER_H
#define VOCALTRAINER_MACOSWORKSPACEDRAWERRESOURCESPROVIDER_H

#include <Logic/WorkspaceDrawerResourcesProvider.h>

class MacOSWorkspaceDrawerResourcesProvider : public WorkspaceDrawerResourcesProvider {
public:
    CppUtils::Bitmap createImageForCharacter(char ch, int fontSize, CppUtils::Color color, Drawer::FontStyle fontStyle, float scaleFactor) const override;
    CppUtils::Bitmap createImageForName(Image image, int widthInPoints, int heightInPoints, float scaleFactor) const override;

    void provideClockFont(int *fontSize, Drawer::FontStyle *style) const override;
    void provideYardStickFont(int *fontSize, Drawer::FontStyle *style) const override;
    void providePianoFont(int *fontSize, Drawer::FontStyle *style) const override;
};


#endif //VOCALTRAINER_MACOSWORKSPACEDRAWERRESOURCESPROVIDER_H
