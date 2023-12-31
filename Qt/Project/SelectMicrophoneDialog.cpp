#include "SelectMicrophoneDialog.h"

#include <QQuickWidget>
#include <QQuickWidget>
#include <QQmlContext>
#include <QQuickItem>
#include <cmath>

#include "Utils/QtUtils.h"
#include "PortAudioUtils.h"
#include "Algorithms.h"
#include "PortAudioInputReader.h"
#include "App/AppSettings.h"
#include "App/VxApp.h"
#include "AudioAverageInputLevelMonitor.h"
#include "Executors.h"

constexpr int DIALOG_WIDTH = 439;
constexpr int DIALOG_HEIGHT = 331;

constexpr int BUFFER_SIZE = 1024;
constexpr int MICROPHONE_MAX_LEVEL = 10;

using std::cout;
using std::endl;

using namespace CppUtils;

SelectMicrophoneDialog::SelectMicrophoneDialog(QWidget* parent, QtCppBridge* cpp) :
        BaseQmlDialog(parent, cpp, "qrc:/qml/Project/SelectMicrophoneDialog.qml") {
    setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);

    QStringList microphoneNames = Transform<QStringList>(PortAudio::getInputDevices(), [] (const PaDeviceInfo* device) {
        return QString::fromLocal8Bit(device->name);
    });
    getContext()->setContextProperty("names", QVariant::fromValue(microphoneNames));

    audioInputReader = new PortAudioInputReader(BUFFER_SIZE,
            false, // outputEnabled
            AppSettings().getMicrophoneDeviceName().data());
    AudioAverageInputLevelMonitor monitor([this] (double level) {
        Executors::ExecuteOnMainThread([=]{
            onInputLevelChanged(level);
        });
    });
    audioInputReader->callbacks.addListener(monitor);
    audioInputReader->start();
}

void SelectMicrophoneDialog::onInputLevelChanged(double level) {
    int microphoneLevel = (int)ceil(level * MICROPHONE_MAX_LEVEL);
    getRootQml()->setProperty("microphoneLevel", microphoneLevel);
}

SelectMicrophoneDialog::~SelectMicrophoneDialog() {
    delete audioInputReader;
}

void SelectMicrophoneDialog::onSelectedMicrophoneIndexChanged(int selectedMicrophoneIndex) {
    const PaDeviceInfo* inputDevice = PortAudio::getInputDevices()[selectedMicrophoneIndex];
    audioInputReader->setDeviceName(inputDevice->name);
}

void SelectMicrophoneDialog::choose(int index) {
    const PaDeviceInfo* inputDevice = PortAudio::getInputDevices()[index];
    AppSettings().setMicrophoneDeviceName(inputDevice->name);
    VxApp::instance()->getAudioInputManager()->setInputDeviceName(inputDevice->name);
    close();
}
