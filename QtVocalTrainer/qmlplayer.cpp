#include "qmlplayer.h"
#include <QVariant>
#include <QJsonObject>

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
    QString prevSource = this->source;
    this->source = source;
    QByteArray local8Bit = source.toLocal8Bit();
    if (local8Bit.startsWith(FILE_URL_PREFIX)) {
        local8Bit.remove(0, FILE_URL_PREFIX_LENGTH);
    }

    init(local8Bit.data());
    prepare();

    cachedVxPitches.reserve(getVxFile().getPitches().size());

    if (prevSource != source) {
        emit sourceChanged(source);
    }
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

QmlVxPitchArray QmlPlayer::getPitchesInTimeRange(double startTime, double endTime) const {
    cachedVxPitches.clear();
    const VxFile &vxFile = getVxFile();
    vxFile.iteratePitchesInTimeRange(startTime, endTime, [&] (const VxPitch& vxPitch) {
        double pitchStartTime = vxFile.ticksToSeconds(vxPitch.startTickNumber);
        double pitchDuration = vxFile.ticksToSeconds(vxPitch.ticksCount);
        cachedVxPitches.push_back(QmlVxPitch(vxPitch.pitch, pitchStartTime, pitchDuration));
    });

    return QmlVxPitchArray(&cachedVxPitches);
}


void QmlPlayer::setQmlBounds(const QJsonValue &bounds) {
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

QJsonValue QmlPlayer::getQmlBounds() const {
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

int QmlVxPitchArray::getPitchesCount() const {
    if (cachedVxPitches == nullptr) {
        return 0;
    }

    return cachedVxPitches->size();
}

const QmlVxPitch &QmlVxPitchArray::at(int index) const {
    return (*cachedVxPitches)[index];
}

QmlVxPitchArray::QmlVxPitchArray(const std::vector<QmlVxPitch> *cachedVxPitches) : cachedVxPitches(cachedVxPitches) {
}
