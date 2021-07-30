//
// Created by Semyon Tikhonenko on 7/28/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import "NativeColorUtils.h"
#include "Color.h"
#include "VocalTrainerColorUtils.h"

using namespace CppUtils;

@implementation NativeColorUtils {

}

+ (UInt32)rotateColorPalette:(CGFloat)angle color:(UInt32)color {
    return VocalTrainerColorUtils::rotateColorPalette(Color::fromRgba(color), angle).toRgbaUint32();
}


@end