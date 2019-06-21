//
// Created by Semyon Tykhonenko on 2019-06-08.
//

#ifndef TEXTIMAGESGENERATOR_ADDLYRICSDIALOG_H
#define TEXTIMAGESGENERATOR_ADDLYRICSDIALOG_H

#include <Utils/BaseQmlDialog.h>

class AddLyricsDialog : public BaseQmlDialog {
    Q_OBJECT
public:
    AddLyricsDialog(QWidget* parent, QtCppBridge* cpp);
    Q_INVOKABLE void add(const QString& text);
private:
};


#endif //TEXTIMAGESGENERATOR_ADDLYRICSDIALOG_H
