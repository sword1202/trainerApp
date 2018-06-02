#ifndef QMLPLAYER_H
#define QMLPLAYER_H

#include <QObject>
#include "MvxPlayer.h"
#include <memory>
#include "qmlvxpitch.h"
#include <QJsonValue>

class Player : public QObject, public MvxPlayer
{
    Q_OBJECT
    QString source;
public:
    explicit Player(QObject *parent = nullptr);

    const QString &getSource() const;
    void setSource(const QString &source);
    void setQmlBounds(const QJsonValue& bounds);
    QJsonValue getQmlBounds() const;

    Q_PROPERTY(QString source READ getSource() WRITE setSource() NOTIFY sourceChanged())
    Q_PROPERTY(bool isPlaying READ isPlaying() NOTIFY isPlayingChanged())
    Q_PROPERTY(double seek READ getSeek() WRITE setSeek() NOTIFY seekChanged())
    Q_PROPERTY(QJsonValue bounds READ getQmlBounds() WRITE setQmlBounds() NOTIFY boundsChanged())
    Q_PROPERTY(double playStartedTime READ getPlayStartedTime() NOTIFY playStartedTimeChanged())
    Q_PROPERTY(double playStartedSeek READ getPlayStartedSeek()  NOTIFY playStartedSeekChanged())
    Q_PROPERTY(double duration READ getDuration() NOTIFY durationChanged())
    Q_PROPERTY(double beatsPerMinute READ getBeatsPerMinute() NOTIFY beatsPerMinuteChanged())

    Q_INVOKABLE void play() override;
    Q_INVOKABLE void pause() override;
    Q_INVOKABLE void stop();

    void onPlaybackStopped() override;

    void onComplete() override;
    void onSeekChanged(double seek) override;
    void onPlaybackStarted() override;

    static Player* instance();

signals:
    void complete();
    void sourceChanged(const QString& newSource);
    void isPlayingChanged(bool isPlaying);
    void seekChanged(double seek);
    void boundsChanged();
    void durationChanged();
    void playStartedTimeChanged();
    void playStartedSeekChanged();
    void beatsPerMinuteChanged();
};

#endif // QMLPLAYER_H