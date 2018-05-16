#include "qmlpitch.h"
QString QmlPitch::getNameAsQString() const {
    return QString(getFullName().data());
}

QmlPitch::QmlPitch(float frequency, double timeInSeconds) : Pitch(frequency),
                                                            timeInSeconds(timeInSeconds) {

}

double QmlPitch::getTimeInSeconds() const {
    return timeInSeconds;
}

QmlPitch::QmlPitch(int perfectFrequencyIndex) :
        Pitch(Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex)),
        timeInSeconds(-1)
{

}
