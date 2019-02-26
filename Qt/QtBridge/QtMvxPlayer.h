#ifndef QMLPLAYER_H
#define QMLPLAYER_H

#include <QObject>
#include "MvxPlayer.h"
#include <memory>
#include <QJsonValue>
#include "WavAudioPlayer.h"
#include "PlaybackBounds.h"

class QtMvxPlayer : public QObject, public MvxPlayer
{
    Q_OBJECT
    QString source;
public:
    explicit QtMvxPlayer(QObject *parent = nullptr);

    const QString &getSource() const;
    void setSource(const QString &source);
    void setQmlBounds(const QJsonValue& bounds);
    QJsonValue getQmlBounds() const;

    Q_PROPERTY(QString source READ getSource() WRITE setSource() NOTIFY sourceChanged())
    Q_PROPERTY(bool isPlaying READ isPlaying() NOTIFY isPlayingChanged())
    Q_PROPERTY(QJsonValue bounds READ getQmlBounds() WRITE setQmlBounds() NOTIFY boundsChanged())
    Q_PROPERTY(double playStartedTime READ getPlayStartedTime() NOTIFY playStartedTimeChanged())
    Q_PROPERTY(double playStartedSeek READ getPlayStartedSeek()  NOTIFY playStartedSeekChanged())
    Q_PROPERTY(double duration READ getDuration() NOTIFY durationChanged())
    Q_PROPERTY(double beatsPerMinute READ getBeatsPerMinute() NOTIFY beatsPerMinuteChanged())
    Q_PROPERTY(int pitchShift READ getPitchShiftInSemiTones() WRITE
            setPitchShiftInSemiTones() NOTIFY pitchShiftChanged())
    Q_PROPERTY(int metronomeEnabled READ isMetronomeEnabled() WRITE
            setMetronomeEnabled() NOTIFY metronomeEnabledChanged())
    Q_PROPERTY(QString artistName READ getArtistName() NOTIFY sourceChanged())
    Q_PROPERTY(QString songTitle READ getSongTitle() NOTIFY sourceChanged())
    Q_PROPERTY(float instrumentalVolume READ getInstrumentalVolume() WRITE setInstrumentalVolume() NOTIFY
            instrumentalVolumeChanged())
    Q_PROPERTY(float pianoVolume READ getPianoVolume() WRITE setPianoVolume() NOTIFY pianoVolumeChanged())

    Q_INVOKABLE bool hasPitchNow(int perfectFrequencyIndex) const;
    Q_INVOKABLE bool hasAnyPitchNow() const;

    Q_INVOKABLE void play() override;
    Q_INVOKABLE void pause() override;
    Q_INVOKABLE void stop();

    Q_INVOKABLE bool canBeShifted(int distance) const override;

    Q_INVOKABLE void seekToNextTact() override;
    Q_INVOKABLE void seekToPrevTact() override;

    Q_INVOKABLE bool hasLyrics() const override;

    void onPlaybackStopped() override;

    void onComplete() override;
    void onPlaybackStarted() override;

    void setPitchShiftInSemiTones(int value) override;
    void setMetronomeEnabled(bool metronomeEnabled) override;

    QString getArtistName() const;
    QString getSongTitle() const;

    void setInstrumentalVolume(float instrumentalVolume) override;

    void setPianoVolume(float pianoVolume) override;

signals:
    void complete();
    void sourceChanged(const QString& newSource);
    void isPlayingChanged(bool isPlaying);
    void boundsChanged();
    void durationChanged();
    void playStartedTimeChanged();
    void playStartedSeekChanged();
    void beatsPerMinuteChanged();
    void pitchShiftChanged();
    void metronomeEnabledChanged();
    void pianoVolumeChanged();
    void instrumentalVolumeChanged();
};

#endif // QMLPLAYER_H