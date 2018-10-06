#include "SelectMicrophoneDialog.h"

#include <QQuickWidget>
#include <QQuickWidget>
#include <QQmlContext>
#include <QQuickItem>

#include "QtUtils/qtutils.h"
#include "PortAudioUtils.h"
#include "Algorithms.h"
#include "PortAudioInputReader.h"
#include "AppSettings.h"
#include "VxApp.h"
#include "AudioAverageInputLevelMonitor.h"
#include "Executors.h"

constexpr int DIALOG_WIDTH = 439;
constexpr int DIALOG_HEIGHT = 331;

constexpr int BUFFER_SIZE = 1024;
constexpr int MICROPHONE_MAX_LEVEL = 10;

using std::cout;
using std::endl;

using namespace CppUtils;

SelectMicrophoneDialog::SelectMicrophoneDialog(QWidget* parent, QmlCppBridge* cpp) : QDialog(parent) {
    setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);

    QQuickWidget* selectMicrophoneDialogView = new QQuickWidget(this);

    QQmlContext *context = selectMicrophoneDialogView->rootContext();
    QStringList microphoneNames = Transform<QStringList>(PortAudio::getInputDevices(), [] (const PaDeviceInfo* device) {
        return QString::fromLocal8Bit(device->name);
    });
    context->setContextProperty("names", QVariant::fromValue(microphoneNames));
    context->setContextProperty("self", this);
    context->setContextProperty("cpp", cpp);

    selectMicrophoneDialogView->setSource(QUrl("qrc:/qml/SelectMicrophoneDialog.qml"));
    rootQml = selectMicrophoneDialogView->rootObject();

    setAttribute(Qt::WA_DeleteOnClose, true);

    audioInputReader = new PortAudioInputReader(BUFFER_SIZE,
            false, // outputEnabled
            AppSettings().getMicrophoneDeviceName().data());
    AudioAverageInputLevelMonitor monitor(BUFFER_SIZE, [this] (double level) {
        Executors::ExecuteOnMainThread([=]{
            onInputLevelChanged(level);
        });
    });
    audioInputReader->callbacks.addListener(monitor);
    audioInputReader->start();

    setModal(true);
}

void SelectMicrophoneDialog::onInputLevelChanged(double level) {
    int microphoneLevel = (int)ceil(level * MICROPHONE_MAX_LEVEL);
    rootQml->setProperty("microphoneLevel", microphoneLevel);
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
