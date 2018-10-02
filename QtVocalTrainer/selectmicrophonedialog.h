#ifndef SELECTMICROPHONEDIALOG_H
#define SELECTMICROPHONEDIALOG_H

#include <QDialog>
#include "QmlCppBridge.h"
#include "AudioInputReader.h"

class SelectMicrophoneDialog : public QDialog
{
    Q_OBJECT
public:
    SelectMicrophoneDialog(QWidget* parent, QmlCppBridge* cpp);
    ~SelectMicrophoneDialog();
    Q_INVOKABLE void choose(int index);
    Q_INVOKABLE void onSelectedMicrophoneIndexChanged(int selectedMicrophoneIndex);
private:
    AudioInputReader* audioInputReader = nullptr;
    QQuickItem* rootQml;

    void onInputLevelChanged(double level);
};

#endif // SELECTMICROPHONEDIALOG_H