#include "appsettings.h"

static const QString MICROPHONE_NAME_KEY = "microphoneDeviceName";
static const QString INPUT_VOLUME_KEY = "inputVolume";
static const QString OUTPUT_VOLUME_KEY = "outputVolume";

AppSettings::AppSettings() {

}

QByteArray AppSettings::getMicrophoneDeviceName() const {
    return value(MICROPHONE_NAME_KEY, 0).toByteArray();
}

void AppSettings::setMicrophoneDeviceName(const QByteArray &name) {
    setValue(MICROPHONE_NAME_KEY, name);
}

float AppSettings::getInputVolume() {
    return value(INPUT_VOLUME_KEY, 1.0).toFloat();
}

void AppSettings::setOutputVolume(float value) {
    setValue(OUTPUT_VOLUME_KEY, value);
}

float AppSettings::getOutputVolume() {
    return value(OUTPUT_VOLUME_KEY, 0.0f).toFloat();
}

void AppSettings::setInputVolume(float value) {
    setValue(INPUT_VOLUME_KEY, value);
}
