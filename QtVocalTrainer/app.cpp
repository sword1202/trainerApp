#include "app.h"
#include "../PitchDetection/PortAudio.h"
#include "QmlCppBridge.h"
#include "qmlpitchinputreader.h"
#include <boost/pool/pool_alloc.hpp>
#include <QQmlContext>

static const QEvent::Type MainLoopEvent = (QEvent::Type)(QEvent::User + 1);

class MainLoopCallbackEvent : public QEvent {
    // avoid intensive memory allocation
    static boost::fast_pool_allocator<MainLoopCallbackEvent> allocator;
public:
    std::function<void()> callback;
    MainLoopCallbackEvent(const std::function<void()>& callback) : QEvent(MainLoopEvent), callback(callback) {
    }

    void* operator new(size_t size) {
        return allocator.allocate();
    }

    void operator delete(void * p) {
        allocator.destroy((MainLoopCallbackEvent*)p);
    }
};

boost::fast_pool_allocator<MainLoopCallbackEvent> MainLoopCallbackEvent::allocator;

QtVxApp::QtVxApp(int argc, char *argv[]) : QApplication(argc, argv), VxApp(new QmlPitchInputReader(), new Player()) {
    PortAudio::init();
#ifdef __APPLE__
    doMacOsPlatformStaff();
#endif

    QObject::connect(Player::instance(), &Player::isPlayingChanged, QmlPitchInputReader::instance(), [] (bool playing) {
        QmlPitchInputReader *instance = QmlPitchInputReader::instance();
        if (playing) {
            instance->start();
        } else {
            instance->stop();
        }
    });

    initInstance(this);
}

bool QtVxApp::event(QEvent *event) {
    if (event->type() == MainLoopEvent) {
        static_cast<MainLoopCallbackEvent*>(event)->callback();
        return true;
    }

 }

void QtVxApp::executeOnMainThread(const std::function<void()> &function) {
    MainLoopCallbackEvent* event = new MainLoopCallbackEvent(function);
    postEvent(this, event);
}

QtVxApp *QtVxApp::instance() {
    return static_cast<QtVxApp*>(QCoreApplication::instance());
}

QtVxApp::~QtVxApp() {
    delete engine;
    PortAudio::terminate();
}
