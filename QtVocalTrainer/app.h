#ifndef APP_H
#define APP_H

#import <QApplication>
#include <QQmlApplicationEngine>
#include "VxApp.h"

class QtVxApp : public QApplication, public VxApp
{
    QQmlApplicationEngine* engine;

#ifdef __APPLE__
    void doMacOsPlatformStaff();
#endif
public:
    QtVxApp(int argc, char *argv[]);
    void executeOnMainThread(const std::function<void()>& callback);
    ~QtVxApp();

    static QtVxApp* instance();

protected:
    virtual bool event(QEvent *event) override;

};

#endif // APP_H
