#include "player.h"
#include <QVariant>
#include <QJsonObject>
#include <iostream>
#include "TimeUtils.h"

using namespace std;

constexpr char FILE_URL_PREFIX[] = "file://";
constexpr int FILE_URL_PREFIX_LENGTH = 7;

Player::Player(QObject *parent) : QObject(parent) {

}

void Player::onComplete() {
    MvxPlayer::onComplete();
    emit complete();
}

void Player::onPlaybackStarted() {
    MvxPlayer::onPlaybackStarted();
    assert(isPlaying());
    emit playStartedTimeChanged();
    emit playStartedSeekChanged();
    emit isPlayingChanged(true);
}

void Player::onPlaybackStopped() {
    MvxPlayer::onPlaybackStopped();
    assert(!isPlaying());
    emit isPlayingChanged(false);
}

const QString &Player::getSource() const {
    return source;
}

void Player::setSource(const QString &source) {
    QString prevSource = this->source;
    if (source == prevSource) {
        return;
    }

    this->source = source;
    QByteArray local8Bit = source.toLocal8Bit();
    if (local8Bit.startsWith(FILE_URL_PREFIX)) {
        local8Bit.remove(0, FILE_URL_PREFIX_LENGTH);
    }

    init(local8Bit.data());
    prepare();

    emit durationChanged();
    emit beatsPerMinuteChanged();
    emit sourceChanged(source);
}

void Player::play() {
    MvxPlayer::play();
}

void Player::pause() {
    if (source.isNull()) {
        return;
    }

    MvxPlayer::pause();
}

void Player::stop() {
    stopAndMoveSeekToBeginning();
}

void Player::onSeekChanged(double seek) {
    emit seekChanged(seek);
}

void Player::setQmlBounds(const QJsonValue &bounds) {
    boost::optional<Bounds> prevBounds = getBounds();
    if (bounds.isUndefined()) {
        setBounds(boost::optional<Bounds>());
    } else {
        setBounds(Bounds(bounds["startSeek"].toDouble(), bounds["endSeek"].toDouble()));
    }
    if (getBounds() != prevBounds) {
        emit boundsChanged();
    }
}

QJsonValue Player::getQmlBounds() const {
    const boost::optional<Bounds> &bounds = getBounds();
    if (!bounds) {
        return QJsonValue::Undefined;
    }

    return QJsonObject
    {
        {"startSeek", bounds->getStartSeek()},
        {"endSeek", bounds->getEndSeek()}
    };
}

bool Player::hasPitchNow(int perfectFrequencyIndex) const {
    return MvxPlayer::hasPitchNow(Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex));
}

bool Player::hasAnyPitchNow() const {
    return MvxPlayer::hasAnyPitchNow();
}