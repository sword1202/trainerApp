#ifndef APP_H
#define APP_H

#ifdef _WIN32
#include <QApplication>
#else
#import <QApplication>
#endif

#include <QQmlApplicationEngine>
#include "MainController.h"
#include "player.h"

class VxApp : public QApplication, public MainController
{
    Q_OBJECT
    QQmlApplicationEngine* engine;
#ifdef __APPLE__
    void doMacOsPlatformStaff();
#endif
public:
    VxApp(int argc, char *argv[]);
    Player* getPlayer() const;
    void executeOnMainThread(const std::function<void()>& callback);
    ~VxApp();

    static VxApp* instance();

signals:
    void mainThreadCallbackPosted(QObject* callback);
};

#endif // APP_H
