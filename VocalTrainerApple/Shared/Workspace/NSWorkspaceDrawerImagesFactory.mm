//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#include "NSWorkspaceDrawerImagesFactory.h"
#import <Logic/Logic-Swift.h>
#import <VocalTrainer-Swift.h>

using namespace CppUtils;

Bitmap NSWorkspaceDrawerImagesFactory::createImageForCharacter(char ch, int fontSize, CppUtils::Color color) const {
    NSImage* nsImage = [NSImage fromText:[NSString stringWithFormat:@"%c", ch] font:[NSFont systemFontOfSize:fontSize] color:color.toNSColor()];
    NSRect imageRect = NSMakeRect(0, 0, nsImage.size.width, nsImage.size.height);
    CGImage* cgImage = [nsImage CGImageForProposedRect:&imageRect context:NULL hints:nil];
    return Bitmap::fromCGImage(cgImage);
}

Bitmap NSWorkspaceDrawerImagesFactory::createImageForName(const char *imageName) const {
    NSImage* nsImage = [NSImage imageNamed:[NSString stringWithCString:imageName]];
    NSRect imageRect = NSMakeRect(0, 0, nsImage.size.width, nsImage.size.height);
    CGImage* cgImage = [nsImage CGImageForProposedRect:&imageRect context:NULL hints:nil];
    return Bitmap::fromCGImage(cgImage);
}
