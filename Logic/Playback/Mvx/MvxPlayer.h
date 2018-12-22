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
#include "PlaybackBounds.h"
#include "PitchesCollection.h"
#include "WavAudioPlayer.h"
#include <array>

class MvxPlayer : public PlayingPitchSequence {
private:

    AudioFilePlayer instrumentalPlayer;
    VxFileAudioPlayer vxPlayer;
    MetronomeAudioPlayer metronomePlayer;
    WavAudioPlayer recordingPlayer;

    std::vector<AudioPlayer*> players;

    std::atomic_bool metronomeEnabled;
    int pauseRequestedCounter = 0;

    PlaybackBounds bounds;
    double playStartedSeek = -1;
    double playStartedTime = -1;
    MvxFile mvxFile;
    // is valid only for recordings
    PitchesCollection* pitchesCollection = nullptr;

    void updateMetronomeVolume();
    void pausePlayer(AudioPlayer* player);
public:
    CppUtils::ListenersSet<bool> isPlayingChangedListeners;
    CppUtils::ListenersSet<> stopRequestedListeners;
    CppUtils::ListenersSet<> playRequestedListeners;
    CppUtils::ListenersSet<> prepareFinishedListeners;
    CppUtils::ListenersSet<const VxFile*> vxFileChangedListeners;
    CppUtils::SynchronizedListenersSet <double> seekChangedListeners;
    CppUtils::ListenersSet<double> seekChangedFromUserListeners;
    CppUtils::ListenersSet<> tonalityChangedListeners;
    CppUtils::ListenersSet<> onCompleteListeners;
    CppUtils::ListenersSet<const PlaybackBounds&> boundsChangedListeners;

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

    const PlaybackBounds &getBounds() const;
    void setBounds(const PlaybackBounds &bounds);

    virtual void onComplete();
    virtual void onPlaybackStarted();
    virtual void onPlaybackStopped();

    double getPlayStartedSeek() const;
    double getPlayStartedTime() const;
    double getDuration() const;

    double getBeatsPerMinute() const;
    double getBeatsPerSecond() const;
    double getTactsPerSecond() const;

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

    bool isRecording() const;

    const std::string& getArtistNameUtf8() const;
    const std::string& getSongTitleUtf8() const;

    const std::string &getInstrumental();

    bool isCompleted() const;

    // The method is valid only for recordigns
    const PitchesCollection* getPitchesCollection();
};


#endif //VOCALTRAINER_MVXPLAYER_H
