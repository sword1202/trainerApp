#ifndef QT_UTILS_HEADER
#define QT_UTILS_HEADER

#include <QObject>
#include <functional>
#include <QFrame>

namespace QtUtils {
    void startRepeatedTimer(QObject *parent, const std::function<bool()> &action, int intervalInMilliseconds);
    QFrame* createVerticalLine(int width, QWidget* parent);
    QByteArray readAllFromFile(const char* fileName);

    void addDynamicPropertyChangedListener(QObject* self, const QByteArray& propertyName,
            const std::function<void(const QVariant& value)>& callback);
}

#endif
