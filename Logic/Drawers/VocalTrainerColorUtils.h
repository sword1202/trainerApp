//
// Created by Semyon Tikhonenko on 7/28/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERCOLORUTILS_H
#define VOCALTRAINER_VOCALTRAINERCOLORUTILS_H

#include "Color.h"

class VocalTrainerColorUtils {
public:
    // A special algorithm for changing color. It changes hue but also adjusts brightness
    // Angle [0,360]
    static CppUtils::Color rotateColorPalette(const CppUtils::Color& color, double angle);
};


#endif //VOCALTRAINER_VOCALTRAINERCOLORUTILS_H
