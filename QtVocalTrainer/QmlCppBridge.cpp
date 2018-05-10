#include "QmlCppBridge.h"
#include <iostream>

QmlCppBridge::QmlCppBridge(QScreen *screen) : screen(screen) {

}

int QmlCppBridge::dp(qreal dp) {
    return qRound(screen->logicalDotsPerInch() * dp);
}

int QmlCppBridge::dpX(qreal dp) {
    return qRound(screen->logicalDotsPerInchX() * dp);
}

int QmlCppBridge::dpY(qreal dp) {
    qreal i = qRound(screen->logicalDotsPerInchY() * dp);
    std::cout<<i;

    return qRound(screen->logicalDotsPerInchY() * dp);
}
