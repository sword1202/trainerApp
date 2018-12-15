#include "VxApp.h"
#include "PortAudioUtils.h"
#include "QtCppBridge.h"
#include "Fonts.h"
#include <QQmlContext>
#include <iostream>
#include <QThread>
#include <QFontDatabase>
#include "AppSettings.h"

class __MainThreadCallback : public QObject {
public:
    std::function<void()> func;

    __MainThreadCallback(const std::function<void()> &func) : QObject(nullptr), func(func) {
    }
};

VxApp::VxApp(int &argc, char *argv[]) : QApplication(argc, argv) {
    PortAudio::init();
    auto microphoneName = AppSettings().getMicrophoneDeviceName();
    MainController::init(new AudioInputManager(microphoneName.data()), new QtMvxPlayer(), new QmlWorkspaceZoomController());
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
    if (QApplication::instance()->thread() == QThread::currentThread()) {
        callback();
        return;
    }

    __MainThreadCallback* holder = new __MainThreadCallback(callback);
    emit mainThreadCallbackPosted(holder);
}

QtMvxPlayer *VxApp::getPlayer() const {
    return static_cast<QtMvxPlayer *>(MainController::getPlayer());
}

VxApp *VxApp::instance() {
    return static_cast<VxApp*>(QCoreApplication::instance());
}

VxApp::~VxApp() {
    PortAudio::terminate();
}
