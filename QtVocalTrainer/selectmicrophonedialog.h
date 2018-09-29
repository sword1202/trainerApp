#ifndef SELECTMICROPHONEDIALOG_H
#define SELECTMICROPHONEDIALOG_H

#include <QDialog>
#include "QmlCppBridge.h"

class SelectMicrophoneDialog : public QDialog
{
    Q_OBJECT
    void onSelectedMicrophoneIndexChanged(int selectedMicrophoneIndex);
public:
    SelectMicrophoneDialog(QWidget* parent, QmlCppBridge* cpp);
    Q_INVOKABLE void choose(int index);
};

#endif // SELECTMICROPHONEDIALOG_H