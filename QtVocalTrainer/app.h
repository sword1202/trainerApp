#ifndef APP_H
#define APP_H

#import <QApplication>
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

    class MainThreadCallback : public QObject {
    public:
        std::function<void()> func;
    };
public:
    VxApp(int argc, char *argv[]);
    Player* getPlayer() const;
    void executeOnMainThread(std::function<void()> callback);
    ~VxApp();

    static VxApp* instance();

signals:
    void mainThreadCallbackPosted(MainThreadCallback* callback);
};

#endif // APP_H
