#include "qmlpitch.h"
QString QmlPitch::getNameAsQString() const {
    return QString(getFullName().data());
}

QmlPitch::QmlPitch(float frequency) : Pitch(frequency) {

}

QmlPitch::QmlPitch(int perfectFrequencyIndex) :
        Pitch(Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex))
{

}
