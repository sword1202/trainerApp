#include "qmltimedpitch.h"

QmlTimedPitch::QmlTimedPitch()
{

}

double QmlTimedPitch::getTimeInSeconds() const {
    return timeInSeconds;
}

QmlTimedPitch::QmlTimedPitch(float frequency, double timeInSeconds) : QmlPitch(frequency), timeInSeconds(timeInSeconds){

}
