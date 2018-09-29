#include <QQuickWidget>
#include "selectmicrophonedialog.h"
#include "PortAudioUtils.h"
#include "Algorithms.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QQuickItem>

constexpr int DIALOG_WIDTH = 439;
constexpr int DIALOG_HEIGHT = 331;

using namespace CppUtils;

SelectMicrophoneDialog::SelectMicrophoneDialog(QWidget* parent, QmlCppBridge* cpp) : QDialog(parent) {
    setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);

    QQuickWidget* selectMicrophoneDialogView = new QQuickWidget(this);
    selectMicrophoneDialogView->rootContext()->setContextProperty("cpp", cpp);
    selectMicrophoneDialogView->setSource(QUrl("qrc:/qml/SelectMicrophoneDialog.qml"));

    QStringList microphoneNames = Transform<QStringList>(PortAudio::getInputDevices(), [] (const PaDeviceInfo* device) {
        return device->name;
    });
    selectMicrophoneDialogView->rootContext()->setContextProperty("names", QVariant::fromValue(microphoneNames));

    auto* root = selectMicrophoneDialogView->rootObject();
    QtUtils::addDynamicPropertyChangedListener(root, "selectedMicrophoneIndex", [this] (const QVariant& value) {
        onSelectedMicrophoneIndexChanged(value.toInt());
    });

    setModal(true);
}

void SelectMicrophoneDialog::onSelectedMicrophoneIndexChanged(int selectedMicrophoneIndex) {

}
