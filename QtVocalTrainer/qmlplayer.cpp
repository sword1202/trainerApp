#include "qmlplayer.h"

constexpr char FILE_URL_PREFIX[] = "file://";
constexpr int FILE_URL_PREFIX_LENGTH = 7;

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
    QByteArray local8Bit = source.toLocal8Bit();
    if (local8Bit.startsWith(FILE_URL_PREFIX)) {
        local8Bit.remove(0, FILE_URL_PREFIX_LENGTH);
    }

    init(local8Bit.data());
    prepare();
    emit sourceChanged(source);
}

void QmlPlayer::play() {
    MvxPlayer::play();
    emit isPlayingChanged(isPlaying());
}

void QmlPlayer::pause() {
    if (source.isNull()) {
        return;
    }

    MvxPlayer::pause();
    emit isPlayingChanged(isPlaying());
}

void QmlPlayer::stop() {
    stopAndMoveSeekToBeginning();
}

void QmlPlayer::onSeekChanged(double seek) {
    emit seekChanged(seek);
}
