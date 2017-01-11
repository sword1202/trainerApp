//
// Created by Semyon Tikhonenko on 1/11/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#import "NSAlert+AlertUtils.h"


@implementation NSAlert (AlertUtils)

+(NSModalResponse)error:(NSString*)message {
    NSAlert *alert = [NSAlert new];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:message];
    [alert setAlertStyle:NSCriticalAlertStyle];
    return [alert runModal];
}

@end