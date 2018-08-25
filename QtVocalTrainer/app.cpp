#include "app.h"
#include "../PitchDetection/PortAudioUtils.h"
#include "QmlCppBridge.h"
#include "qmlpitchinputreader.h"
#include "fonts.h"
#include <QQmlContext>
#include <iostream>
#include <QThread>
#include <QFontDatabase>

class __MainThreadCallback : public QObject {
public:
    std::function<void()> func;

    __MainThreadCallback(const std::function<void()> &func) : QObject(nullptr), func(func) {
    }
};

VxApp::VxApp(int &argc, char *argv[]) : QApplication(argc, argv), MainController(new QmlPitchInputReader(), new Player(), new QmlZoomController()) {
    PortAudio::init();
#ifdef __APPLE__
    doMacOsPlatformStaff();
#endif
    initInstance(this);

    connect(this, &VxApp::mainThreadCallbackPosted, this, [] (QObject* object) {
        assert(QApplication::instance()->thread() == QThread::currentThread());
        static_cast<__MainThreadCallback*>(object)->func();
        delete object;
    }, Qt::QueuedConnection);

    QFontDatabase::addApplicationFontFromData(Fonts::latoRegular());
    QFontDatabase::addApplicationFontFromData(Fonts::latoBold());
}

void VxApp::executeOnMainThread(const std::function<void()>& callback) {
    __MainThreadCallback* holder = new __MainThreadCallback(callback);
    emit mainThreadCallbackPosted(holder);
}

Player *VxApp::getPlayer() const {
    return static_cast<Player *>(MainController::getPlayer());
}

VxApp *VxApp::instance() {
    return static_cast<VxApp*>(QCoreApplication::instance());
}

VxApp::~VxApp() {
    //delete engine;
    PortAudio::terminate();
}
