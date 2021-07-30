//
// Created by Semyon Tikhonenko on 7/28/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#include "VocalTrainerColorUtils.h"

using namespace CppUtils;

static double colorSum(const Color& color) {
    return int(color.r()) + int(color.g()) + int(color.b());
};

Color VocalTrainerColorUtils::rotateColorPalette(const Color& color, double angle) {
    double h, s, v;
    color.getHSV(&h, &s, &v);
    h += angle;
    Color newColor = Color::fromHSV(h, s, v, color.a() / 255.0);

    double newColorSum = colorSum(newColor);
    double maxColorSum = colorSum(Color::white());
    double colorSum = ::colorSum(color);
    double k = 1.0 - (newColorSum - colorSum) / maxColorSum;

    return Color::fromHSV(h, s, v * k, color.a() / 255.0);
}
