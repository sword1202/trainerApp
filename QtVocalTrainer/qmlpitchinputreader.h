#ifndef QMLPITCHINPUTREADER_H
#define QMLPITCHINPUTREADER_H

#include <QObject>
#include "../PitchDetection/PitchInputReader.h"
#include "qmlpitch.h"
#include <deque>

class QmlPitchInputReader : public QObject
{
    Q_OBJECT
    PitchInputReader* pitchInputReader = nullptr;
    AudioInputReader* audioInputReader = nullptr;

    std::deque<float> frequencies;
    std::deque<double> times;
    bool running;
    double savedPitchesTimeLimit = 10.0;
public:
    explicit QmlPitchInputReader(QObject *parent = nullptr);
    ~QmlPitchInputReader();
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE QmlPitch getPitchAt(int index);

    Q_PROPERTY(int pitchesCount READ getPitchesCount())
    Q_PROPERTY(bool running READ isRunning())
    Q_PROPERTY(float threshold READ getThreshold WRITE setThreshold NOTIFY thresholdChanged)

    int getPitchesCount();

    bool isRunning() const;
    void setThreshold(float threshold);
    float getThreshold() const;

    double getSavedPitchesTimeLimit();
    void setSavedPitchesTimeLimit(double savedPitchesTimeLimit);

signals:
    void pitchDetected(const QmlPitch& pitch);
    void thresholdChanged(float threshold);
    void savedPitchesTimeLimitChanged();
};

#endif // QMLPITCHINPUTREADER_H
