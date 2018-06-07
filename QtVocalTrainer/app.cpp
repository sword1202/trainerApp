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

App::App(int argc, char *argv[]) : QApplication(argc, argv) {
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
}

bool App::event(QEvent *event) {
    if (event->type() == MainLoopEvent) {
        static_cast<MainLoopCallbackEvent*>(event)->callback();
        return true;
    }

    return QGuiApplication::event(event);
}

void App::executeOnMainThread(const std::function<void()> &function) {
    MainLoopCallbackEvent* event = new MainLoopCallbackEvent(function);
    postEvent(this, event);
}

App *App::instance() {
    return static_cast<App*>(QCoreApplication::instance());
}

App::~App() {
    delete engine;
    PortAudio::terminate();
}
