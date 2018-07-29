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

    void updateMetronomeVolume();
    void pausePlayer(AudioPlayer* player);
public:
    CppUtils::ListenersSet<bool> isPlayingChangedListeners;
    CppUtils::ListenersSet<> stopRequestedListeners;
    CppUtils::ListenersSet<> playRequestedListeners;
    CppUtils::ListenersSet<> prepareFinishedListeners;
    CppUtils::ListenersSet<const VxFile*> vxFileChangedListeners;
    CppUtils::ListenersSet<double> seekChangedListeners;
    CppUtils::ListenersSet<double> seekChangedFromUserListeners;
    CppUtils::ListenersSet<> tonalityChangedListeners;

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
};


#endif //VOCALTRAINER_MVXPLAYER_H
