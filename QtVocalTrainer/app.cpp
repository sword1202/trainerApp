#include "app.h"
#include "../PitchDetection/PortAudio.h"
#include "QmlCppBridge.h"
#include "qmlpitchinputreader.h"
#include <boost/pool/pool_alloc.hpp>
#include <QQmlContext>
#include <iostream>

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

VxApp::VxApp(int argc, char *argv[]) : QApplication(argc, argv), MainController(new QmlPitchInputReader(), new Player(), new QmlZoomController()) {
    PortAudio::init();
#ifdef __APPLE__
    doMacOsPlatformStaff();
#endif
    initInstance(this);
}

bool VxApp::event(QEvent *event) {
    if (event->type() == MainLoopEvent) {
        static_cast<MainLoopCallbackEvent*>(event)->callback();
        return true;
    }

    return false;
}

void VxApp::executeOnMainThread(const std::function<void()> &function) {
    MainLoopCallbackEvent* event = new MainLoopCallbackEvent(function);
    postEvent(this, event);
}

Player *VxApp::getPlayer() const {
    return static_cast<Player *>(MainController::getPlayer());
}

VxApp *VxApp::instance() {
    return static_cast<VxApp*>(QCoreApplication::instance());
}

VxApp::~VxApp() {
    delete engine;
    PortAudio::terminate();
}
