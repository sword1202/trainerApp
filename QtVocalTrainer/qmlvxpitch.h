#ifndef QMLVXPITCH_H
#define QMLVXPITCH_H


#include <QObject>
#include "qmlpitch.h"
#include "VxPitch.h"

class QmlVxPitch : QmlPitch
{
    Q_GADGET
    double startTime;
    double duration;
public:
    Q_PROPERTY(double startTime READ getStartTime())
    Q_PROPERTY(double endTime READ getEndTime())
    Q_PROPERTY(double duration READ getDuration())

    QmlVxPitch();
    QmlVxPitch(const Pitch& pitch, double startTime, double duration);

    double getStartTime() const;
    double getDuration() const;
    double getEndTime() const;
};

#endif // QMLVXPITCH_H
