#include "qtutils.h"
#include <QTimer>
#include <qfile.h>
#include <QEvent>
#include "assert.h"
#include <QVariant>

namespace QtUtils {
    void startRepeatedTimer(QObject *parent, const std::function<bool()> &action, int intervalInMilliseconds) {
        assert(action);
        assert(intervalInMilliseconds >= 0);
        QTimer* timer = new QTimer(parent);
        timer->setInterval(intervalInMilliseconds);
        QObject::connect(timer, &QTimer::timeout, [=] {
            if (!action()) {
                timer->stop();
            }
        });
        timer->start(intervalInMilliseconds);
    }

    QFrame* createVerticalLine(int width, QWidget* parent) {
        QFrame* frame = new QFrame(parent);
        frame->setFrameShape(QFrame::VLine);
        frame->setLineWidth(width);
        frame->setFixedWidth(width);
        return frame;
    }

    QByteArray readAllFromFile(const char* fileName) {
        QFile file(fileName);
        file.open(QFile::ReadOnly);
        return file.readAll();
    }

    class Watcher : public QObject {
        std::function<void(const QVariant& value)> callback;
        QByteArray propertyName;
    public:
        Watcher(QObject *parent, const std::function<void(const QVariant& value)>& callback,
                const QByteArray& propertyName) :
        QObject(parent),
        propertyName(propertyName),
        callback(callback) {}

        virtual ~Watcher() {}

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override {
            if (event->type() == QEvent::DynamicPropertyChange) {
                QDynamicPropertyChangeEvent *const propEvent = static_cast<QDynamicPropertyChangeEvent*>(event);
                if (propEvent->propertyName() == propertyName) {
                    callback(obj->property(propertyName.data()));
                }
            }

            return false;
        }
    };

    void addDynamicPropertyChangedListener(QObject* self, const QByteArray& propertyName,
            const std::function<void(const QVariant& value)>& callback) {
        self->installEventFilter(new Watcher(self, callback, propertyName));
    }
}
