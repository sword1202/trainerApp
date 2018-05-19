#include "qmlplayer.h"

QmlPlayer::QmlPlayer(QObject *parent) : QObject(parent) {

}

void QmlPlayer::onComplete() {
    MvxPlayer::onComplete();
    emit complete();
}

const QString &QmlPlayer::getSource() const {
    return source;
}

void QmlPlayer::setSource(const QString &source) {
    this->source = source;
    init(source.toLocal8Bit().data());
    prepare();
    emit sourceChanged(source);
}

void QmlPlayer::play() {
    MvxPlayer::play();
    emit isPlayingChanged(isPlaying());
}

void QmlPlayer::pause() {
    MvxPlayer::pause();
    emit isPlayingChanged(isPlaying());
}

void QmlPlayer::stop() {
    stopAndMoveSeekToBeginning();
}

void QmlPlayer::onSeekChanged(double seek) {
    emit seekChanged(seek);
}
