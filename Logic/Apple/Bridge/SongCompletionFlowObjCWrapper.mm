//
// Created by Semyon Tikhonenko on 5/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import "SongCompletionFlowObjCWrapper.h"

@implementation SongCompletionFlowObjCWrapper {
    SongCompletionFlow* _cpp;
}
- (instancetype)initWithCpp:(SongCompletionFlow *)cpp {
    self = [super init];
    if (self) {
        _cpp = cpp;
    }

    return self;
}

- (void)tryAgain {
    _cpp->tryAgain();
}

- (void)save {
    _cpp->save();
}

- (void)listen {
    _cpp->listen();
}

@end