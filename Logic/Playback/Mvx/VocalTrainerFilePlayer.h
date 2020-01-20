//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERFILEPLAYER_H
#define VOCALTRAINER_VOCALTRAINERFILEPLAYER_H

#include <iostream>
#include "VocalPart.h"
#include "MvxFile.h"
#include "VocalPartAudioPlayer.h"
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
#include "AudioAverageInputLevelMonitor.h"
#include "Timer.h"
#include "Rewindable.h"
#include "BeatsPerMinuteProvider.h"
#include "VocalTrainerFile.h"

class VocalTrainerFilePlayer : public PlayingPitchSequence, public Rewindable, public BeatsPerMinuteProvider {
private:

    AudioFilePlayer instrumentalPlayer;
    VocalPartAudioPlayer vocalPartPianoPlayer;
    MetronomeAudioPlayer metronomePlayer;
    WavAudioPlayer recordingPlayer;

    std::vector<AudioPlayer*> players;

    std::atomic_bool metronomeEnabled;
    int pauseRequestedCounter = 0;

    PlaybackBounds bounds;
    double playStartedSeek = -1;
    double playStartedTime = -1;
    VocalTrainerFile* file = nullptr;
    bool destroyMvxFileOnDestructor;
    // is valid only for recordings
    PitchesCollection* pitchesCollection = nullptr;

    // Save tonality changes while playback, not used with recordings
    std::map<double, int> tonalityChanges;

    AudioAverageInputLevelMonitor* recordingLevelMonitor = nullptr;
    Lyrics::Snapshot lastLyricsSnapshot;

    void updateMetronomeVolume();
    void pausePlayer(AudioPlayer* player);
public:
    CppUtils::ListenersSet<bool> isPlayingChangedListeners;
    CppUtils::ListenersSet<> stopRequestedListeners;
    CppUtils::ListenersSet<> playRequestedListeners;
    CppUtils::ListenersSet<> prepareFinishedListeners;
    CppUtils::ListenersSet<const VocalPart*> vocalPartChangedListeners;
    CppUtils::SynchronizedListenersSet <double> seekChangedListeners;
    CppUtils::ListenersSet<double> seekChangedFromUserListeners;
    CppUtils::ListenersSet<> tonalityChangedListeners;
    CppUtils::ListenersSet<> onCompleteListeners;
    CppUtils::ListenersSet<const PlaybackBounds&> boundsChangedListeners;
    CppUtils::SynchronizedListenersSet<double> recordingVoiceLevelListeners;
    CppUtils::ListenersSet<> lyricsChangedListeners;
    CppUtils::ListenersSet<> currentLyricsLinesChangedListeners;

    VocalTrainerFilePlayer();
    virtual ~VocalTrainerFilePlayer();
    void setSource(std::istream &is);
    void setSource(const char *filePath);
    void setSource(VocalTrainerFile *file, bool destroyFileOnDestructor = true);
    void prepare();
    virtual void setInstrumentalVolume(float instrumentalVolume);
    virtual void setVocalPartPianoVolume(float pianoVolume);
    float getInstrumentalVolume() const;
    float getVocalPianoVolume() const;
    void setRecordingVolume(float volume);
    virtual void pause();
    virtual void stopAndMoveSeekToBeginning();
    virtual void play();
    virtual bool canBeShifted(int distance) const;
    virtual int getPitchShiftInSemiTones() const;
    virtual void setPitchShiftInSemiTones(int value);
    bool isPlaying() const;
    void setSeek(double value) override;
    double getSeek() const override;
    virtual void seekToNextTact();
    virtual void seekToPrevTact();
    const VocalPart* getVocalPart() const;
    const VocalTrainerFile &getFile() const;
    void editLyrics(const std::function<void(Lyrics *lyrics)> &editAction);

    const PlaybackBounds &getBounds() const;
    void setBounds(const PlaybackBounds &bounds);

    virtual void onComplete();
    virtual void onPlaybackStarted();
    virtual void onPlaybackStopped();

    double getPlayStartedSeek() const;
    double getPlayStartedTime() const;
    double getDuration() const;

    double getBeatsPerMinute() const override;

    double getTempoFactor() const;

    void setTempoFactor(double tempoFactor);

    bool hasPitchNow(const Pitch& pitch) const;
    bool hasAnyPitchNow() const;

    void setMetronomeSoundData(std::string&& data);
    bool isMetronomeSoundDataSet() const;

    bool isMetronomeEnabled() const;
    virtual void setMetronomeEnabled(bool metronomeEnabled);

    bool isRecording() const;

    const std::string& getArtistNameUtf8() const;
    const std::string& getSongTitleUtf8() const;

    const std::string &getInstrumental();

    bool isCompleted() const;

    // The method is valid only for recordigns
    const PitchesCollection* getPitchesCollection();

    virtual bool hasLyrics() const;
    int getLyricsLinesCount() const;
    const std::string& getLyricsTextForPart(int partIndex) const;

    const AudioPlayer& getInstrumentalPlayer() const;
    const VocalPartAudioPlayer& getVocalPartPlayer() const;

    const std::map<double, int> &getTonalityChanges() const;
};


#endif //VOCALTRAINER_VOCALTRAINERFILEPLAYER_H
