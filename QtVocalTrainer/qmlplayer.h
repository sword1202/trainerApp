#ifndef QMLPLAYER_H
#define QMLPLAYER_H

#include <QObject>
#include "MvxPlayer.h"
#include <memory>

class QmlPlayer : public QObject, public MvxPlayer
{
    Q_OBJECT
    QString source;
public:
    explicit QmlPlayer(QObject *parent = nullptr);

    const QString &getSource() const;
    void setSource(const QString &source);

    Q_PROPERTY(QString source READ getSource() WRITE setSource() NOTIFY sourceChanged())
    Q_PROPERTY(bool isPlaying READ isPlaying() NOTIFY isPlayingChanged())
    Q_PROPERTY(double seek READ getSeek() WRITE setSeek() NOTIFY seekChanged())

    Q_INVOKABLE void play() override;
    Q_INVOKABLE void pause() override;
    Q_INVOKABLE void stop();

    void onComplete() override;
    void onSeekChanged(double seek) override;

signals:
    void complete();
    void sourceChanged(const QString& newSource);
    void isPlayingChanged(bool isPlaying);
    void seekChanged(double seek);
};

#endif // QMLPLAYER_H