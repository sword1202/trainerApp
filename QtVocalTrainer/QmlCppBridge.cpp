#include "QmlCppBridge.h"

QmlCppBridge::QmlCppBridge(QScreen *screen) : screen(screen) {

}

int QmlCppBridge::dp(qreal dp) {
    return qRound(screen->logicalDotsPerInch() * dp / 25.4f);
}

int QmlCppBridge::dpX(qreal dp) {
    return qRound(screen->logicalDotsPerInchX() * dp / 25.4f);
}

int QmlCppBridge::dpY(qreal dp) {
    return qRound(screen->logicalDotsPerInchY() * dp / 25.4f);
}
