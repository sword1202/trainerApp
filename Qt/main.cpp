#include <QApplication>
#include <QFontDatabase>

#include "App/VxApp.h"
#include "QtBridge/QtPitch.h"
#include "QtBridge/QtMvxPlayer.h"
#include "Welcome/WelcomeWindow.h"

int main(int argc, char *argv[])
{
    VxApp::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication::setAttribute(Qt::AA_UseOpenGLES, false);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, false);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);

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
