#include <utility>

#include "QtUtils.h"
#include <QTimer>
#include <qfile.h>
#include <QEvent>
#include "assert.h"
#include <QVariant>
#include <QResizeEvent>

namespace QtUtils {
    void StartRepeatedTimer(QObject *parent, const std::function<bool()> &action, int intervalInMilliseconds) {
        assert(action);
        assert(intervalInMilliseconds >= 0);
        auto * timer = new QTimer(parent);
        timer->setInterval(intervalInMilliseconds);
        QObject::connect(timer, &QTimer::timeout, [=] {
            if (!action()) {
                timer->stop();
            }
        });
        timer->start(intervalInMilliseconds);
    }

    QFrame* CreateVerticalLine(int width, QWidget *parent) {
        QFrame* frame = new QFrame(parent);
        frame->setFrameShape(QFrame::VLine);
        frame->setLineWidth(width);
        frame->setFixedWidth(width);
        return frame;
    }

    QByteArray ReadAllFromFile(const char *fileName) {
        QFile file(fileName);
        file.open(QFile::ReadOnly);
        return file.readAll();
    }

    class Watcher : public QObject {
        std::function<void(const QVariant& value)> callback;
        QByteArray propertyName;
    public:
        Watcher(QObject *parent, std::function<void(const QVariant &value)> callback,
                QByteArray propertyName) :
        QObject(parent),
        propertyName(std::move(propertyName)),
        callback(std::move(callback)) {}

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override {
            if (event->type() == QEvent::DynamicPropertyChange) {
                auto *const propEvent = static_cast<QDynamicPropertyChangeEvent*>(event);
                if (propEvent->propertyName() == propertyName) {
                    callback(obj->property(propertyName.data()));
                    return true;
                }
            }

            return QObject::eventFilter(obj, event);
        }
    };

    void AddDynamicPropertyChangedListener(QObject *self, const QByteArray &propertyName,
                                           const std::function<void(const QVariant &value)> &callback) {
        self->installEventFilter(new Watcher(self, callback, propertyName));
    }

    QString QStringFromUtf8(const std::string& utf8) {
        return QString::fromUtf8(utf8.data(), utf8.size());
    }

    void AddResizeListener(QWidget* widget, const std::function<void(int, int)>& listener) {
        auto* resizeListener = new _ResizeListener(widget, listener);
        widget->installEventFilter(resizeListener);
    }

    bool _ResizeListener::eventFilter(QObject *watched, QEvent *event) {
        if (event->type() == QEvent::Resize) {
            auto* resizeEvent = static_cast<QResizeEvent *>(event);
            QSize size = resizeEvent->size();
            listener(size.width(), size.height());
        }

        return false;
    }

    _ResizeListener::_ResizeListener(QObject *parent, const std::function<void(int, int)> &listener) : QObject(parent),
    listener(listener) {}
}
