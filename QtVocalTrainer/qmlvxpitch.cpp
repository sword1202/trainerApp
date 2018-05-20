#include "qmlvxpitch.h"

QmlVxPitch::QmlVxPitch()
{

}

QmlVxPitch::QmlVxPitch(const Pitch &pitch, double startTime, double duration) :
        QmlPitch(pitch.getPerfectFrequencyIndex()), startTime(startTime), duration(duration) {
    assert(startTime > 0 && duration > 0);
}

double QmlVxPitch::getStartTime() const {
    return startTime;
}

double QmlVxPitch::getDuration() const {
    return duration;
}

double QmlVxPitch::getEndTime() const {
    return startTime + duration;
}
