#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include "AppSettings.h"

static const QString MICROPHONE_NAME_KEY = "microphoneDeviceName";
static const QString INPUT_VOLUME_KEY = "inputVolume";
static const QString OUTPUT_VOLUME_KEY = "outputVolume";
static const QString RECORDINGS_LIST_KEY = "recordingsList";
static const QString PROJECTS_LIST_KEY = "projectsList";

AppSettings::AppSettings() = default;

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

QStringList AppSettings::getRecordings() {
    return value(RECORDINGS_LIST_KEY, QStringList()).value<QStringList>();
}

void AppSettings::setRecordings(const QStringList &recordings) {
    setValue(RECORDINGS_LIST_KEY, recordings);
}

QStringList AppSettings::getProjects() {
    return value(PROJECTS_LIST_KEY, QStringList()).value<QStringList>();
}

void AppSettings::setProjects(const QStringList &projects) {
    setValue(PROJECTS_LIST_KEY, projects);
}

void AppSettings::addProject(const QString &filePath) {
    auto projects = getProjects();
    projects.removeAll(filePath);
    projects.prepend(filePath);
    setProjects(projects);
}

void AppSettings::addRecording(const QString &filePath) {
    auto recordings = getRecordings();
    recordings.removeAll(filePath);
    recordings.prepend(filePath);
    setRecordings(recordings);
}

#pragma clang diagnostic pop