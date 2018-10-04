#ifndef QMLTIMEDPITCH_H
#define QMLTIMEDPITCH_H

#include "QmlPitch.h"

class QmlTimedPitch : public QmlPitch
{
    Q_GADGET
    double timeInSeconds;
public:
    Q_PROPERTY(double time READ getTimeInSeconds())

    QmlTimedPitch();
    QmlTimedPitch(float frequency, double timeInSeconds);
    QmlTimedPitch(const Pitch& pitch, double timeInSeconds);

    double getTimeInSeconds() const;
};

#endif // QMLTIMEDPITCH_H