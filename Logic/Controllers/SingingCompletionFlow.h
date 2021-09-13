//
// Created by Semyon Tikhonenko on 5/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SINGINGCOMPLETIONFLOW_H
#define VOCALTRAINER_SINGINGCOMPLETIONFLOW_H


class SingingCompletionFlow {
public:
    virtual void tryAgain() = 0;
    virtual void save() = 0;
    virtual void listen() = 0;
    virtual ~SingingCompletionFlow() = default;
};


#endif //VOCALTRAINER_SINGINGCOMPLETIONFLOW_H
