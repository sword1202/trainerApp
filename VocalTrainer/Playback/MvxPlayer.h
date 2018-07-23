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
#include "AudioFilePlayer.h"
#include <boost/optional.hpp>
#include <memory>
#include <functional>
#include "ListenersSet.h"
#include "PlayingPitchSequence.h"

class MvxPlayer : public PlayingPitchSequence {
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

    typedef typename CppUtils::ListenersSet<bool>::Listener IsPlayingChangedListener;
    typedef typename CppUtils::ListenersSet<>::Listener PrepareFinishedListener;
    typedef typename CppUtils::ListenersSet<const VxFile*>::Listener VxFileChangedListener;
    typedef typename CppUtils::ListenersSet<double>::Listener SeekChangedListener;
    typedef typename CppUtils::ListenersSet<>::Listener TonalityChangedListener;

private:

    AudioFilePlayer instrumentalPlayer;
    VxFileAudioPlayer vxPlayer;
    boost::optional<Bounds> bounds;
    double playStartedSeek = -1;
    double playStartedTime = -1;
    double beatsPerMinute;

    CppUtils::ListenersSet<bool> isPlayingChangedListeners;
    CppUtils::ListenersSet<> prepareFinishedListeners;
    CppUtils::ListenersSet<const VxFile*> vxFileChangedListeners;
    CppUtils::ListenersSet<double> seekChangedListeners;
    CppUtils::ListenersSet<> tonalityChangedListeners;
public:
    MvxPlayer();
    virtual ~MvxPlayer();
    void init(std::istream& is);
    void init(const char* filePath);
    void prepare();
    void setInstrumentalVolume(float instrumentalVolume);
    void setPianoVolume(float pianoVolume);
    virtual void pause();
    virtual void stopAndMoveSeekToBeginning();
    virtual void play();
    virtual bool canBeShifted(int distance) const;
    virtual int getPitchShiftInSemiTones() const;
    virtual void setPitchShiftInSemiTones(int value);
    bool isPlaying() const;
    void setSeek(double value);
    double getSeek() const;
    const VxFile* getVxFile() const;

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

    double getTempoFactor() const;

    void setTempoFactor(double tempoFactor);

    bool hasPitchNow(const Pitch& pitch) const;
    bool hasAnyPitchNow() const;

    // The listener is executed on Main thread
    int addIsPlayingChangedListener(const IsPlayingChangedListener& listener);
    void removeIsPlayingChangedListener(int id);

    // The listener is executed on Main thread
    int addPrepareFinishedListener(const PrepareFinishedListener& listener);
    void removePrepareFinishedListener(int id);

    // The listener is executed on Main thread
    int addVxFileChangedListener(const VxFileChangedListener& listener);
    void removeVxFileChangedListener(int id);

    // The listener is executed on Audio callback thread
    int addSeekChangedListener(const SeekChangedListener& listener);
    void removeSeekChangedListener(int id);

    // The listener is executed on Audio callback thread
    int addTonalityChangedListener(const TonalityChangedListener& listener);
    void removeTonalityChangedListener(int id);
};


#endif //VOCALTRAINER_MVXPLAYER_H
