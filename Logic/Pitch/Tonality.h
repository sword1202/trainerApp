//
// Created by Semyon Tikhonenko on 3/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_TONALITY_H
#define VOCALTRAINER_TONALITY_H

#include <string>

class Tonality {
    int pitchInOctaveIndex = 0;
    bool major = false;
public:
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & pitchInOctaveIndex;
        ar & major;
    }

    Tonality(int pitchInOctaveIndex, bool isMajor);
    static Tonality parse(const std::string& str);
    Tonality();

    int getPitchInOctaveIndex() const;
    bool isMajor() const;
};


#endif //VOCALTRAINER_TONALITY_H
