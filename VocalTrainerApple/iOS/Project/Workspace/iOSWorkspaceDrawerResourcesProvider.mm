//
// Created by Semyon Tikhonenko on 12/16/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <VocalTraineriOS-Swift.h>
#include "iOSWorkspaceDrawerResourcesProvider.h"

using namespace CppUtils;

Bitmap iOSWorkspaceDrawerResourcesProvider::createImageForCharacter(
        char ch, int fontSize, CppUtils::Color color, Drawer::FontStyle fontStyle, float scaleFactor
) const {
    UIFontWeight weight = UIFontWeightRegular;
    switch (fontStyle) {
        case Drawer::NORMAL:
            weight = UIFontWeightRegular;
            break;
        case Drawer::BOLD:
            weight = UIFontWeightBold;
            break;
        case Drawer::SEMIBOLD:
            weight = UIFontWeightSemibold;
            break;
    }

    UIImage* image = [UIImage fromText:[NSString stringWithFormat:@"%c", ch]
                                  font:[UIFont systemFontOfSize:fontSize weight:weight]
                             textColor:color.toUIColor()];
    return Bitmap::fromCGImage(image.CGImage);
}

Bitmap iOSWorkspaceDrawerResourcesProvider::createImageForName(
        WorkspaceDrawerResourcesProvider::Image image, int widthInPoints, int heightInPoints, float scaleFactor
) const {
    if (image == playHeadTriangle) {
        UIImage *uiImage = [UIImage imageNamed:@"Playhead"];
        uiImage = [uiImage resizedToPointSize:CGSizeMake(widthInPoints, heightInPoints)];
        return Bitmap::fromCGImage(uiImage.CGImage);
    } else {
        return Bitmap();
    }
}
