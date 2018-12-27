#ifndef QT_UTILS_HEADER
#define QT_UTILS_HEADER

#include <QObject>
#include <functional>
#include <QFrame>
#ifdef __OBJC__
#import <AppKit/AppKit.h>
#endif

namespace QtUtils {
    void StartRepeatedTimer(QObject *parent, const std::function<bool()> &action, int intervalInMilliseconds);
    QFrame* CreateVerticalLine(int width, QWidget *parent);
    QByteArray ReadAllFromFile(const char *fileName);

    void AddDynamicPropertyChangedListener(QObject *self, const QByteArray &propertyName,
                                           const std::function<void(const QVariant &value)> &callback);

    QString QStringFromUtf8(const std::string& utf8);

    class _ResizeListener : public QObject {
        std::function<void(int, int)> listener;
    public:
        _ResizeListener(QObject *parent, const std::function<void(int, int)> &listener);
        bool eventFilter(QObject *watched, QEvent *event) override;
    };

    void AddResizeListener(QWidget* widget, const std::function<void(int, int)>& listener);

#ifdef __OBJC__
    NSView* getNSView(QWidget* widget);
#endif
}

#endif
