//
// Created by Semyon Tykhonenko on 8/29/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SEEKER_H
#define VOCALTRAINER_SEEKER_H


class Seeker {
public:
    virtual double getSeek() const = 0;
    virtual void setSeek(double value) = 0;
};


#endif //VOCALTRAINER_SEEKER_H
