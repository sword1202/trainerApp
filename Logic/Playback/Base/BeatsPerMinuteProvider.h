//
// Created by Semyon Tykhonenko on 8/31/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_BEATSPERMINUTEPROVIDER_H
#define VOCALTRAINER_BEATSPERMINUTEPROVIDER_H


class BeatsPerMinuteProvider {
public:
    virtual double getBeatsPerMinute() const = 0;
    virtual int getBeatsInBar() const = 0;

    inline double getBeatDuration() const {
        return 60 / getBeatsPerMinute();
    }

    inline double getBarDuration() const {
        return getBeatsInBar() * getBeatDuration();
    }

    inline double getBeatsPerSecond() const {
        return getBeatsPerMinute() / 60.0;
    }
};


#endif //VOCALTRAINER_BEATSPERMINUTEPROVIDER_H
