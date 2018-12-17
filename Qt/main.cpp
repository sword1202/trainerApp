#include <QApplication>
#include <QFontDatabase>

#include "App/VxApp.h"
#include "QtBridge/QtPitch.h"
#include "QtBridge/QtMvxPlayer.h"
#include "Welcome/WelcomeWindow.h"

// Avoid metal and opengl collision performance issues, render qml with software engine
#ifdef __APPLE__
#define USE_OPEN_GL false
#else
#define USE_OPEN_GL true
#endif

int main(int argc, char *argv[])
{
    VxApp::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication::setAttribute(Qt::AA_UseOpenGLES, false);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, !USE_OPEN_GL);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL, USE_OPEN_GL);

    QCoreApplication::setOrganizationName("NeboroSoft");
    QCoreApplication::setOrganizationDomain("neborosoft.com");
    QCoreApplication::setApplicationName("Vocal Trainer");

    qRegisterMetaType<QtPitch>("Pitch");
    qmlRegisterType<QtMvxPlayer>();

    VxApp a(argc, argv);
    VxApp::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    auto* welcomeWindow = new WelcomeWindow();
    welcomeWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    welcomeWindow->show();

    return VxApp::exec();
}
