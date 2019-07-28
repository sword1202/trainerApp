//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_MACOSWORKSPACEDRAWERSETUPDELEGATE_H
#define VOCALTRAINER_MACOSWORKSPACEDRAWERSETUPDELEGATE_H

#include <Logic/WorkspaceDrawerSetupDelegate.h>

class MacOSWorkspaceDrawerSetupDelegate : public WorkspaceDrawerSetupDelegate {
public:
    CppUtils::Bitmap createImageForCharacter(char ch, int fontSize, CppUtils::Color color, Drawer::FontStyle fontStyle) const override;
    CppUtils::Bitmap createImageForName(const char *imageName) const override;

    void provideClockFont(int *fontSize, Drawer::FontStyle *style) const override;
    void provideYardStickFont(int *fontSize, Drawer::FontStyle *style) const override;
    void providePianoFont(int *fontSize, Drawer::FontStyle *style) const override;
};


#endif //VOCALTRAINER_MACOSWORKSPACEDRAWERSETUPDELEGATE_H
