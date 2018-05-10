#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QQuickView>
#include "MainWindow.h"

class QmlCppBridge : public QObject
{
    Q_OBJECT
    MainWindow* view;
public:
    int windowWidth();
    int windowHeight();
    Q_PROPERTY(int windowWidth READ windowWidth()  NOTIFY resize())
    Q_PROPERTY(int windowHeight READ windowHeight() NOTIFY resize())

    Q_INVOKABLE int dp(qreal dp);
    Q_INVOKABLE int dpX(qreal dp);
    Q_INVOKABLE int dpY(qreal dp);

    QmlCppBridge(MainWindow* view);

signals:
    void resize();
};

#endif // QMLCPPBRIDGE_H
