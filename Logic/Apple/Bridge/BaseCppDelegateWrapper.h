//
// Created by Semyon Tikhonenko on 9/17/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_BASECPPDELEGATEWRAPPER_H
#define VOCALTRAINER_BASECPPDELEGATEWRAPPER_H

#import <Foundation/Foundation.h>

class BaseCppDelegateWrapper {
protected:
    NSHashTable<id>* delegates;
public:
    BaseCppDelegateWrapper() {
        delegates = [NSHashTable weakObjectsHashTable];
    }

    void addDelegate(id delegate) {
        [delegates addObject:delegate];
    }

    void removeDelegate(id delegate) {
        [delegates removeObject:delegate];
    }
};



#endif //VOCALTRAINER_BASECPPDELEGATEWRAPPER_H
