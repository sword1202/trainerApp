#ifndef QMLPLAYER_H
#define QMLPLAYER_H

#include <QObject>
#include "MvxPlayer.h"
#include <memory>
#include "qmlvxpitch.h"
#include <QJsonValue>

class QmlVxPitchArray {
    Q_GADGET
    const std::vector<QmlVxPitch>* cachedVxPitches = nullptr;
public:
    Q_PROPERTY(int count READ getPitchesCount())
    Q_INVOKABLE const QmlVxPitch& at(int index) const;

    QmlVxPitchArray() = default;
    QmlVxPitchArray(const std::vector<QmlVxPitch> *cachedVxPitches);

    int getPitchesCount() const;
};

class QmlPlayer : public QObject, public MvxPlayer
{
    Q_OBJECT
    QString source;
    mutable std::vector<QmlVxPitch> cachedVxPitches;
public:
    explicit QmlPlayer(QObject *parent = nullptr);

    const QString &getSource() const;
    void setSource(const QString &source);
    void setQmlBounds(const QJsonValue& bounds);
    QJsonValue getQmlBounds() const;

    Q_PROPERTY(QString source READ getSource() WRITE setSource() NOTIFY sourceChanged())
    Q_PROPERTY(bool isPlaying READ isPlaying() NOTIFY isPlayingChanged())
    Q_PROPERTY(double seek READ getSeek() WRITE setSeek() NOTIFY seekChanged())
    Q_PROPERTY(QJsonValue bounds READ getQmlBounds() WRITE setQmlBounds() NOTIFY boundsChanged())
    Q_PROPERTY(double playStartedTime READ getPlayStartedTime())
    Q_PROPERTY(double playStartedSeek READ getPlayStartedSeek())
    Q_PROPERTY(double duration READ getDuration())
    Q_PROPERTY(double beatsPerMinute READ getBeatsPerMinute())

    Q_INVOKABLE void play() override;
    Q_INVOKABLE void pause() override;
    Q_INVOKABLE void stop();

    void onPlaybackStopped() override;

    void onComplete() override;
    void onSeekChanged(double seek) override;

    void onPlaybackStarted() override;

    Q_INVOKABLE QmlVxPitchArray getPitchesInTimeRange(double startTime, double endTime) const;

signals:
    void complete();
    void sourceChanged(const QString& newSource);
    void isPlayingChanged(bool isPlaying);
    void seekChanged(double seek);
    void boundsChanged();
};

#endif // QMLPLAYER_H