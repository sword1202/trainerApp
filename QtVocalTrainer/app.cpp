#include "app.h"
#include "../PitchDetection/PortAudio.h"
#include "QmlCppBridge.h"
#include "qmlpitchinputreader.h"
#include <QQmlContext>
#include <iostream>

VxApp::VxApp(int argc, char *argv[]) : QApplication(argc, argv), MainController(new QmlPitchInputReader(), new Player(), new QmlZoomController()) {
    PortAudio::init();
#ifdef __APPLE__
    doMacOsPlatformStaff();
#endif
    initInstance(this);

    connect(this, &VxApp::mainThreadCallbackPosted, [] (std::function<void()> callback) {
        callback();
    });
}

void VxApp::executeOnMainThread(std::function<void()> function) {
    emit mainThreadCallbackPosted(function);
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
