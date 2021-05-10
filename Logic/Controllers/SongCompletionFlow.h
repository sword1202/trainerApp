//
// Created by Semyon Tikhonenko on 5/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SONGCOMPLETIONFLOW_H
#define VOCALTRAINER_SONGCOMPLETIONFLOW_H


class SongCompletionFlow {
public:
    virtual void tryAgain() = 0;
    virtual void save() = 0;
    virtual void listen() = 0;
    virtual ~SongCompletionFlow() = default;
};


#endif //VOCALTRAINER_SONGCOMPLETIONFLOW_H
