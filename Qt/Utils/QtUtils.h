#ifndef QT_UTILS_HEADER
#define QT_UTILS_HEADER

#include <QObject>
#include <functional>
#include <QFrame>
#ifdef __OBJC__
#import <AppKit/AppKit.h>
#endif

namespace QtUtils {
    void startRepeatedTimer(QObject *parent, const std::function<bool()> &action, int intervalInMilliseconds);
    QFrame* createVerticalLine(int width, QWidget* parent);
    QByteArray readAllFromFile(const char* fileName);

    void addDynamicPropertyChangedListener(QObject* self, const QByteArray& propertyName,
            const std::function<void(const QVariant& value)>& callback);

#ifdef __OBJC__
    NSView* getNSView(QWidget* widget);
#endif
}

#endif
