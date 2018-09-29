#ifndef SELECTMICROPHONEDIALOG_H
#define SELECTMICROPHONEDIALOG_H

#include <QDialog>
#include "QmlCppBridge.h"

class SelectMicrophoneDialog : public QDialog
{
    void onSelectedMicrophoneIndexChanged(int selectedMicrophoneIndex);
public:
    SelectMicrophoneDialog(QWidget* parent, QmlCppBridge* cpp);
};

#endif // SELECTMICROPHONEDIALOG_H