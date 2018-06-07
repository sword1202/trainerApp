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
#include <memory>

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

        bool operator==(const Bounds &rhs) const;
        bool operator!=(const Bounds &rhs) const;
    };
private:

    std::unique_ptr<AudioPlayer, AudioPlayer::Deleter> instrumentalPlayer;
    std::unique_ptr<VxFileAudioPlayer, AudioPlayer::Deleter> vxPlayer;
    boost::optional<Bounds> bounds;
    double playStartedSeek = -1;
    double playStartedTime = -1;
    double beatsPerMinute;

    void setupVxPlayerDesyncHandler() const;
    void setupInstrumentalPlayerDesyncHandler() const;
public:

    virtual ~MvxPlayer();
    void init(std::istream& is);
    void init(const char* filePath);
    void prepare();
    void setInstrumentalVolume(float instrumentalVolume);
    void setPianoVolume(float pianoVolume);
    virtual void pause();
    virtual void stopAndMoveSeekToBeginning();
    virtual void play();
    bool isPlaying() const;
    void setSeek(double value);
    double getSeek() const;
    const VxFile& getVxFile() const;

    const boost::optional<Bounds> &getBounds() const;
    void setBounds(const boost::optional<Bounds> &bounds);

    virtual void onComplete();
    virtual void onSeekChanged(double seek);
    virtual void onPlaybackStarted();
    virtual void onPlaybackStopped();

    double getPlayStartedSeek() const;
    double getPlayStartedTime() const;
    double getDuration() const;

    double getBeatsPerMinute() const;

    bool hasPitchNow(const Pitch& pitch) const;
    bool hasAnyPitchNow() const;
};


#endif //VOCALTRAINER_MVXPLAYER_H
