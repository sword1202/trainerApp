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
#include "MetronomeAudioPlayer.h"
#include <array>

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
    typedef typename CppUtils::ListenersSet<>::Listener StopRequestedListener;
    typedef typename CppUtils::ListenersSet<const VxFile*>::Listener VxFileChangedListener;
    typedef typename CppUtils::ListenersSet<double>::Listener SeekChangedListener;
    typedef typename CppUtils::ListenersSet<>::Listener TonalityChangedListener;

private:

    AudioFilePlayer instrumentalPlayer;
    VxFileAudioPlayer vxPlayer;
    MetronomeAudioPlayer metronomePlayer;

    std::array<AudioPlayer*, 3> players;

    std::atomic_bool metronomeEnabled;
    int pauseRequestedCounter = 0;

    boost::optional<Bounds> bounds;
    double playStartedSeek = -1;
    double playStartedTime = -1;
    double beatsPerMinute;

    CppUtils::ListenersSet<bool> isPlayingChangedListeners;
    CppUtils::ListenersSet<> stopRequestedListeners;
    CppUtils::ListenersSet<> playRequestedListeners;
    CppUtils::ListenersSet<> prepareFinishedListeners;
    CppUtils::ListenersSet<const VxFile*> vxFileChangedListeners;
    CppUtils::ListenersSet<double> seekChangedListeners;
    CppUtils::ListenersSet<double> seekChangedFromUserListeners;
    CppUtils::ListenersSet<> tonalityChangedListeners;

    void updateMetronomeVolume();
    void pausePlayer(AudioPlayer* player);
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
    virtual void seekToNextTact();
    virtual void seekToPrevTact();
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

    void setMetronomeSoundData(std::string&& data);
    bool isMetronomeSoundDataSet() const;

    bool isMetronomeEnabled() const;
    virtual void setMetronomeEnabled(bool metronomeEnabled);

    double getBeatDuration() const;
    double getTactDuration() const;

    int addIsPlayingChangedListener(const IsPlayingChangedListener& listener);
    void removeIsPlayingChangedListener(int id);

    int addStopRequestedListener(const StopRequestedListener& listener);
    void removeStopRequestedListener(int id);

    int addPlayRequestedListener(const StopRequestedListener& listener);
    void removePlayRequestedListener(int id);

    int addPrepareFinishedListener(const PrepareFinishedListener& listener);
    void removePrepareFinishedListener(int id);

    int addVxFileChangedListener(const VxFileChangedListener& listener);
    void removeVxFileChangedListener(int id);

    int addSeekChangedListener(const SeekChangedListener& listener);
    void removeSeekChangedListener(int id);

    int addSeekChangedFromUserListener(const SeekChangedListener& listener);
    void removeSeekChangedFromUserListener(int id);

    // The listener is executed on Audio callback thread
    int addTonalityChangedListener(const TonalityChangedListener& listener);
    void removeTonalityChangedListener(int id);
};


#endif //VOCALTRAINER_MVXPLAYER_H
