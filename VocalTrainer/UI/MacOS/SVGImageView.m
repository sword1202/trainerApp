//
// Created by Semyon Tikhonenko on 7/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "SVGImageView.h"
#import "IJSVG.h"


@implementation SVGImageView {

}
- (instancetype)initWithFileName:(NSString *)fileName andFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.fileName = fileName;
    }

    return self;
}


- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    IJSVG * svg = [IJSVG svgNamed:_fileName];
    [svg drawInRect:dirtyRect];
}


@end