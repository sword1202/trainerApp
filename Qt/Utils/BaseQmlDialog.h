//
// Created by Semyon Tykhonenko on 2019-06-08.
//

#ifndef TEXTIMAGESGENERATOR_BASEQMLDIALOG_H
#define TEXTIMAGESGENERATOR_BASEQMLDIALOG_H

#include <QDialog>
#include <QtBridge/QtCppBridge.h>

class BaseQmlDialog : public QDialog {
    QQuickItem* rootQml;
    QQmlContext* context;
protected:
    QQuickItem *getRootQml();
    QQmlContext *getContext();

public:
    BaseQmlDialog(QWidget* parent, QtCppBridge* cpp, const char* qmlPath);
};


#endif //TEXTIMAGESGENERATOR_BASEQMLDIALOG_H
