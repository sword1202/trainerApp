#include "qmlpitch.h"

QmlPitch::QmlPitch(const Pitch &pitch) : Pitch(pitch) {

}

QString QmlPitch::getNameAsQString() const {
    return QString(getName());
}
