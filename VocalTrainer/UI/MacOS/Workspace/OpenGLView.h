//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

@interface OpenGLView : NSOpenGLView
- (CVReturn)draw:(const CVTimeStamp *)outputTime;
@end