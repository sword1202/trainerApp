//
// Created by Semyon Tikhonenko on 1/11/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#import "NSString+StringUtils.h"
#import <ctype.h>

@implementation NSString (StringUtils)
-(BOOL)isEqualToStringIgnoreCase:(NSString *)aString {
    NSUInteger length = aString.length;
    if (length != self.length) {
        return NO;
    }

    for (NSUInteger i = 0; i < length; ++i) {
        if (tolower([aString characterAtIndex:i]) != tolower([self characterAtIndex:i])) {
            return NO;
        }
    }

    return YES;
}
@end