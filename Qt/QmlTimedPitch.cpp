#include "QmlTimedPitch.h"

QmlTimedPitch::QmlTimedPitch()
{

}

double QmlTimedPitch::getTimeInSeconds() const {
    return timeInSeconds;
}

QmlTimedPitch::QmlTimedPitch(float frequency, double timeInSeconds) : QmlPitch(frequency), timeInSeconds(timeInSeconds){

}

QmlTimedPitch::QmlTimedPitch(const Pitch& pitch, double timeInSeconds) : QmlPitch(pitch), timeInSeconds(timeInSeconds) {

}
