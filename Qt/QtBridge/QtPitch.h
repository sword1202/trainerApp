#ifndef QMLPITCH_H
#define QMLPITCH_H

#include <QObject>
#include "Pitch.h"

class QtPitch : public Pitch
{
    Q_GADGET
public:

    Q_PROPERTY(QString name READ getNameAsQString())
    Q_PROPERTY(int perfectFrequencyIndex READ getPerfectFrequencyIndex())
    Q_PROPERTY(int indexInOctave READ getPitchInOctaveIndex())
    Q_PROPERTY(int octave READ getOctave())
    Q_PROPERTY(float frequency READ getFrequency())
    Q_PROPERTY(bool isValid READ isValid())
    Q_PROPERTY(int whiteIndex READ getWhiteIndex())

    // Calculated using formula: n = log(fn / f0, a), where
    // A = 2**(1/24),
    // f0 - pitch lower bound frequency,
    // fn - pitch actual frequency
    // returns value (-1.0f, 1.0f). Where 0 - perfect frequency
    Q_PROPERTY(float distanceToPerfectFrequency READ getDistanceToPerfectFrequency())
    // returns value (0.0f, 2.0f). Where 1.0 - perfect frequency
    Q_PROPERTY(float distanceFromLowerBound READ getDistanceFromLowerBound())

public:
    QtPitch(int perfectFrequencyIndex);
    QtPitch(float frequency);
    QtPitch(const Pitch& pitch);
    QtPitch(const QtPitch& pitch) = default;
    QtPitch() = default;

    QString getNameAsQString() const;
};

#endif // QMLPITCH_H
