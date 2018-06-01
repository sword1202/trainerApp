#ifndef QT_UTILS_HEADER
#define QT_UTILS_HEADER

#include <QObject>
#include <functional>

namespace QtUtils {
    void startRepeatedTimer(QObject *parent, const std::function<bool()> &action, int intervalInMilliseconds);
}

#endif
