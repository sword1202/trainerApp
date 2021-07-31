//
// Created by Semyon Tikhonenko on 7/30/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_TIMESIGNATURE_H
#define VOCALTRAINER_TIMESIGNATURE_H

class TimeSignature {
    int beatDuration;
    int numberOfBeatsInBar;
public:
    TimeSignature();

    TimeSignature(int numberOfBeatsInBar, int beatDuration);

    int getBeatDuration() const;
    int getNumberOfBeatsInBar() const;

    template <typename Archive>
    void saveOrLoad(Archive& archive, bool save) {
        archive(beatDuration);
        archive(numberOfBeatsInBar);
    }
};


#endif //VOCALTRAINER_TIMESIGNATURE_H
