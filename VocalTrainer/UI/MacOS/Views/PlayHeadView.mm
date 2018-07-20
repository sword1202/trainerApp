//
// Created by Semyon Tikhonenko on 7/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "PlayHeadView.h"
#import "IJSVG.h"
#include "QuartzDrawer.h"

constexpr CGFloat kTriangleSize = 11;

@implementation PlayHeadView {

}
- (instancetype)initWithHeight:(CGFloat)height {
    self = [super initWithFrame:CGRectMake(0, 0, kTriangleSize, height)];
    if (self) {

    }

    return self;
}

- (void)setPosition:(CGFloat)position {
    _position = position;
    self.frame.origin.x = position - kTriangleSize;
}


- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];

    IJSVG * svg = [IJSVG svgNamed:@"play_head_triangle"];
    [svg drawInRect:CGRectMake(0, 0, kTriangleSize, kTriangleSize)];

    QuartzDrawer drawer;
    drawer.setStrokeColor({0x24 ,0x23 ,0x2D ,0xff});
    drawer.drawVerticalLine(kTriangleSize / 2.0, 0, dirtyRect.size.height);
}

@end