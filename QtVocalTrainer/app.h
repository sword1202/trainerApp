#ifndef APP_H
#define APP_H

#import <QApplication>
#include <QQmlApplicationEngine>
#include "MainController.h"
#include "player.h"

class VxApp : public QApplication, public MainController
{
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

protected:
    virtual bool event(QEvent *event) override;

};

#endif // APP_H
