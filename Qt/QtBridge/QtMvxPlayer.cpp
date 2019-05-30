#include "QtMvxPlayer.h"
#include <QVariant>
#include <QJsonObject>
#include <iostream>
#include "TimeUtils.h"
#include "Utils/QtUtils.h"
#include "WAVFile.h"
#include "PlaybackBounds.h"

using namespace std;
using namespace CppUtils;

constexpr char FILE_URL_PREFIX[] = "file://";
constexpr int FILE_URL_PREFIX_LENGTH = 7;

QtMvxPlayer::QtMvxPlayer(QObject *parent) : QObject(parent) {

}

void QtMvxPlayer::onComplete() {
    MvxPlayer::onComplete();
    emit complete();
}

void QtMvxPlayer::onPlaybackStarted() {
    MvxPlayer::onPlaybackStarted();
    assert(isPlaying());
    emit playStartedTimeChanged();
    emit playStartedSeekChanged();
    emit isPlayingChanged(true);
}

void QtMvxPlayer::onPlaybackStopped() {
    MvxPlayer::onPlaybackStopped();
    assert(!isPlaying());
    emit isPlayingChanged(false);
}

const QString &QtMvxPlayer::getSource() const {
    return source;
}

void QtMvxPlayer::setSource(const QString &source) {
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

    if (!isMetronomeSoundDataSet()) {
        std::string metronomeData = QtUtils::ReadAllFromFile(":qml/sounds/metronome.wav").toStdString();
        setMetronomeSoundData(std::move(metronomeData));
    }

    prepare();

    emit durationChanged();
    emit beatsPerMinuteChanged();
    emit sourceChanged(source);
}

void QtMvxPlayer::play() {
    MvxPlayer::play();
}

void QtMvxPlayer::pause() {
    if (source.isNull()) {
        return;
    }

    MvxPlayer::pause();
}

void QtMvxPlayer::stop() {
    stopAndMoveSeekToBeginning();
}

void QtMvxPlayer::setQmlBounds(const QJsonValue &bounds) {
    const PlaybackBounds& prevBounds = getBounds();
    if (bounds.isUndefined()) {
        setBounds(PlaybackBounds());
    } else {
        setBounds(PlaybackBounds(bounds["startSeek"].toDouble(), bounds["endSeek"].toDouble()));
    }
    if (getBounds() != prevBounds) {
        emit boundsChanged();
    }
}

QJsonValue QtMvxPlayer::getQmlBounds() const {
    const PlaybackBounds &bounds = getBounds();
    if (!bounds) {
        return QJsonValue::Undefined;
    }

    return QJsonObject
    {
        {"startSeek", bounds.getStartSeek()},
        {"endSeek", bounds.getEndSeek()}
    };
}

bool QtMvxPlayer::hasPitchNow(int perfectFrequencyIndex) const {
    return MvxPlayer::hasPitchNow(Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex));
}

bool QtMvxPlayer::hasAnyPitchNow() const {
    return MvxPlayer::hasAnyPitchNow();
}

void QtMvxPlayer::setPitchShiftInSemiTones(int value) {
    if (value == getPitchShiftInSemiTones()) {
        return;
    }

    MvxPlayer::setPitchShiftInSemiTones(value);
    emit pitchShiftChanged();
}

bool QtMvxPlayer::canBeShifted(int distance) const {
    return MvxPlayer::canBeShifted(distance);
}

void QtMvxPlayer::setMetronomeEnabled(bool metronomeEnabled) {
    MvxPlayer::setMetronomeEnabled(metronomeEnabled);
    emit metronomeEnabledChanged();
}

void QtMvxPlayer::seekToNextTact() {
    MvxPlayer::seekToNextTact();
}

void QtMvxPlayer::seekToPrevTact() {
    MvxPlayer::seekToPrevTact();
}

QString QtMvxPlayer::getArtistName() const {
    return QString::fromUtf8(getArtistNameUtf8().data());
}

QString QtMvxPlayer::getSongTitle() const {
    return QString::fromUtf8(getSongTitleUtf8().data());
}

bool QtMvxPlayer::hasLyrics() const {
    return MvxPlayer::hasLyrics();
}

void QtMvxPlayer::setInstrumentalVolume(float instrumentalVolume) {
    MvxPlayer::setInstrumentalVolume(instrumentalVolume);
    emit instrumentalVolumeChanged();
}

void QtMvxPlayer::setVocalPartPianoVolume(float pianoVolume) {
    MvxPlayer::setVocalPartPianoVolume(pianoVolume);
    emit pianoVolumeChanged();
}
