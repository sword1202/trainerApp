#ifndef QMLPITCHINPUTREADER_H
#define QMLPITCHINPUTREADER_H

#include <QObject>
#include "../PitchDetection/PitchInputReader.h"
#include "qmlpitch.h"

class QmlPitchInputReader : public QObject
{
    Q_OBJECT
    PitchInputReader* pitchInputReader = nullptr;
    AudioInputReader* audioInputReader = nullptr;
public:
    explicit QmlPitchInputReader(QObject *parent = nullptr);
    ~QmlPitchInputReader();

signals:
    void pitchDetected(const QmlPitch& pitch);
};

#endif // QMLPITCHINPUTREADER_H
