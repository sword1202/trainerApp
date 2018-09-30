#include "appsettings.h"

static const QString MICROPHONE_NAME_KEY = "microphoneDeviceName";

AppSettings::AppSettings() {

}

QByteArray AppSettings::getMicrophoneDeviceName() const {
    return value(MICROPHONE_NAME_KEY, 0).toByteArray();
}

void AppSettings::setMicrophoneDeviceName(const QByteArray &name) {
    setValue(MICROPHONE_NAME_KEY, name);
}
