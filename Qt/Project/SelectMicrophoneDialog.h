#ifndef SELECTMICROPHONEDIALOG_H
#define SELECTMICROPHONEDIALOG_H

#include <Utils/BaseQmlDialog.h>
#include "AudioInputReader.h"

class SelectMicrophoneDialog : public BaseQmlDialog
{
    Q_OBJECT
public:
    SelectMicrophoneDialog(QWidget* parent, QtCppBridge* cpp);
    ~SelectMicrophoneDialog() override;
    Q_INVOKABLE void choose(int index);
    Q_INVOKABLE void onSelectedMicrophoneIndexChanged(int selectedMicrophoneIndex);
private:
    AudioInputReader* audioInputReader = nullptr;

    void onInputLevelChanged(double level);
};

#endif // SELECTMICROPHONEDIALOG_H