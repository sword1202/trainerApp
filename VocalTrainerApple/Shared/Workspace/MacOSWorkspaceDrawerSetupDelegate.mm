//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#include "MacOSWorkspaceDrawerSetupDelegate.h"
#import <Logic/Logic-Swift.h>
#import <VocalTrainer-Swift.h>

using namespace CppUtils;

CppUtils::Bitmap MacOSWorkspaceDrawerSetupDelegate::createImageForCharacter(char ch, int fontSize, CppUtils::Color color, Drawer::FontStyle fontStyle) const {
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
                                   color:color.toNSColor()];
    NSRect imageRect = NSMakeRect(0, 0, nsImage.size.width, nsImage.size.height);
    CGImage* cgImage = [nsImage CGImageForProposedRect:&imageRect context:NULL hints:nil];
    return Bitmap::fromCGImage(cgImage);
}

Bitmap MacOSWorkspaceDrawerSetupDelegate::createImageForName(const char *imageName) const {
    NSImage* nsImage = [NSImage imageNamed:[NSString stringWithCString:imageName]];
    NSRect imageRect = NSMakeRect(0, 0, nsImage.size.width, nsImage.size.height);
    CGImage* cgImage = [nsImage CGImageForProposedRect:&imageRect context:NULL hints:nil];
    return Bitmap::fromCGImage(cgImage);
}

void MacOSWorkspaceDrawerSetupDelegate::provideClockFont(int *fontSize, Drawer::FontStyle *style) const {
}

void MacOSWorkspaceDrawerSetupDelegate::provideYardStickFont(int *fontSize, Drawer::FontStyle *style) const {
}

void MacOSWorkspaceDrawerSetupDelegate::providePianoFont(int *fontSize, Drawer::FontStyle *style) const {
    *style = Drawer::SEMIBOLD;
}
