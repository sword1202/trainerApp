#ifndef SELECTMICROPHONEDIALOG_H
#define SELECTMICROPHONEDIALOG_H

#include <QDialog>
#include "QtCppBridge.h"
#include "AudioInputReader.h"

class SelectMicrophoneDialog : public QDialog
{
    Q_OBJECT
public:
    SelectMicrophoneDialog(QWidget* parent, QtCppBridge* cpp);
    ~SelectMicrophoneDialog();
    Q_INVOKABLE void choose(int index);
    Q_INVOKABLE void onSelectedMicrophoneIndexChanged(int selectedMicrophoneIndex);
private:
    AudioInputReader* audioInputReader = nullptr;
    QQuickItem* rootQml;

    void onInputLevelChanged(double level);
};

#endif // SELECTMICROPHONEDIALOG_H