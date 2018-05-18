//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXPLAYER_H
#define VOCALTRAINER_MVXPLAYER_H

#include <iostream>
#include "VxFile.h"
#include "MvxFile.h"
#include "VxFileAudioPlayer.h"
#include <boost/optional.hpp>

class MvxPlayer {
public:
    class Bounds {
        double startSeek;
        double endSeek;
    public:
        Bounds(double startSeek, double endSeek);
        double getStartSeek() const;
        double getEndSeek() const;
        bool isInside(double value) const;
    };
private:
    AudioPlayer* instrumentalPlayer;
    VxFileAudioPlayer* vxPlayer;
    boost::optional<Bounds> bounds;

    void init(std::istream& is);

    void setupVxPlayerDesyncHandler() const;
    void setupInstrumentalPlayerDesyncHandler() const;
public:

    ~MvxPlayer();
    MvxPlayer(const char* filePath);
    MvxPlayer(std::istream& is);
    void prepare();
    void setInstrumentalVolume(float instrumentalVolume);
    void setPianoVolume(float pianoVolume);
    void pause();
    void stopAndMoveSeekToBeginning();
    void play();
    bool isPlaying() const;
    void seek(double value);
    const VxFile& getVxFile() const;

    const boost::optional<Bounds> &getBounds() const;
    void setBounds(const boost::optional<Bounds> &bounds);
};


#endif //VOCALTRAINER_MVXPLAYER_H
