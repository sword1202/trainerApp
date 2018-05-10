#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QScreen>

class QmlCppBridge : public QObject
{
    Q_OBJECT
    QScreen* screen;
public:
    Q_INVOKABLE int dp(qreal dp);
    Q_INVOKABLE int dpX(qreal dp);
    Q_INVOKABLE int dpY(qreal dp);
    QmlCppBridge(QScreen *screen);
};

#endif // QMLCPPBRIDGE_H
