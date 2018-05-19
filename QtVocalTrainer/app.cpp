#include "app.h"
#include "../PitchDetection/PortAudio.h"
#include <boost/pool/pool_alloc.hpp>

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

App::App(int argc, char *argv[]) : QGuiApplication(argc, argv) {
    PortAudio::init();
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
    PortAudio::terminate();
}
