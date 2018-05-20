#ifndef QMLTIMEDPITCH_H
#define QMLTIMEDPITCH_H

#include "qmlpitch.h"

class QmlTimedPitch : public QmlPitch
{
    Q_GADGET
    double timeInSeconds;
public:
    Q_PROPERTY(double time READ getTimeInSeconds())

    QmlTimedPitch();
    QmlTimedPitch(float frequency, double timeInSeconds);

    double getTimeInSeconds() const;
};

#endif // QMLTIMEDPITCH_H