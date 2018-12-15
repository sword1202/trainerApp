#include "QtPitch.h"
QString QtPitch::getNameAsQString() const {
    return QString(getFullName().data());
}

QtPitch::QtPitch(float frequency) : Pitch(frequency) {

}

QtPitch::QtPitch(int perfectFrequencyIndex) :
        Pitch(Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex))
{

}

QtPitch::QtPitch(const Pitch &pitch) : Pitch(pitch) {

}
