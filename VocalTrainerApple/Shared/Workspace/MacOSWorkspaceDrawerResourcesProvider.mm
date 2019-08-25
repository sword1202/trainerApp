//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#include "MacOSWorkspaceDrawerResourcesProvider.h"
#import <Logic/Logic-Swift.h>
#import <VocalTrainer-Swift.h>

using namespace CppUtils;

CppUtils::Bitmap MacOSWorkspaceDrawerResourcesProvider::createImageForCharacter(char ch, int fontSize, CppUtils::Color color, Drawer::FontStyle fontStyle, float scaleFactor) const {
    NSFontWeight weight;
    switch (fontStyle) {
        case Drawer::BOLD:
            weight = NSFontWeightBold;
            break;
        case Drawer::SEMIBOLD:
            weight = NSFontWeightSemibold;
            break;
        default:
            weight = NSFontWeightRegular;
    }
    NSImage* nsImage = [NSImage fromText:[NSString stringWithFormat:@"%c", ch]
            font:[NSFont systemFontOfSize:fontSize weight:weight]
                                   color:color.toNSColor() scaleFactor:CGFloat(scaleFactor)];
    NSRect imageRect = NSMakeRect(0, 0, nsImage.size.width, nsImage.size.height);
    CGImage* cgImage = [nsImage CGImageForProposedRect:&imageRect context:NULL hints:nil];
    return Bitmap::fromCGImage(cgImage);
}

CppUtils::Bitmap MacOSWorkspaceDrawerResourcesProvider::createImageForName(Image image, int widthInPoints, int heightInPoints, float scaleFactor) const {
    NSImage* nsImage;
    if (image == instrumentalTrackButton) {
        nsImage = [MacOSWorkspaceDrawerDelegateHelper drawTrackButtonWithText:@"Instrumental track"
                                                                     size:CGSizeMake(widthInPoints, heightInPoints)
                                                                 scaleFactor:CGFloat(scaleFactor)];
    } else if (image == pianoTrackButton) {
        nsImage = [MacOSWorkspaceDrawerDelegateHelper drawTrackButtonWithText:@"Vocal track"
                                                                     size:CGSizeMake(widthInPoints, heightInPoints)
                                                                 scaleFactor:CGFloat(scaleFactor)];
    } else {
        nsImage = [NSImage imageNamed:[NSString stringWithCString:imageNames[image].data()]];
    }

    NSRect imageRect = NSMakeRect(0, 0, nsImage.size.width, nsImage.size.height);
    CGImage* cgImage = [nsImage CGImageForProposedRect:&imageRect context:NULL hints:nil];
    return Bitmap::fromCGImage(cgImage);
}

void MacOSWorkspaceDrawerResourcesProvider::provideClockFont(int *fontSize, Drawer::FontStyle *style) const {
}

void MacOSWorkspaceDrawerResourcesProvider::provideYardStickFont(int *fontSize, Drawer::FontStyle *style) const {
}

void MacOSWorkspaceDrawerResourcesProvider::providePianoFont(int *fontSize, Drawer::FontStyle *style) const {
    *style = Drawer::SEMIBOLD;
}
