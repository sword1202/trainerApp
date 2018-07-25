#ifndef QMLPITCHINPUTREADER_H
#define QMLPITCHINPUTREADER_H

#include <QObject>
#include "../PitchDetection/PitchInputReaderCollector.h"
#include "VxPitchInputReader.h"
#include "qmltimedpitch.h"
#include <deque>

class QmlPitchInputReader : public QObject, public VxPitchInputReader
{
    Q_OBJECT
public:
    explicit QmlPitchInputReader(QObject *parent = nullptr);
    Q_INVOKABLE void start() override;
    Q_INVOKABLE void stop() override;
    Q_INVOKABLE QmlTimedPitch pitchAt(int index);
    Q_INVOKABLE QmlTimedPitch getLastDetectedPitch();

    Q_PROPERTY(int pitchesCount READ getPitchesCount())
    Q_PROPERTY(bool running READ isRunning())
    Q_PROPERTY(float threshold READ getThreshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(QmlTimedPitch lastDetectedPitch READ getLastDetectedPitch NOTIFY pitchDetected)

    void pitchDetected(float frequency, double time) override;

    void setThreshold(float threshold) override;

signals:
    void pitchDetected(const QmlTimedPitch& pitch);
    void thresholdChanged(float threshold);
    void savedPitchesTimeLimitChanged();
};

#endif // QMLPITCHINPUTREADER_H
