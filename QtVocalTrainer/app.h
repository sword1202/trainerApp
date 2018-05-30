#ifndef APP_H
#define APP_H

#import <QApplication>
#include <QQmlApplicationEngine>

class App : public QApplication
{
    QQmlApplicationEngine* engine;

#ifdef __APPLE__
    void doMacOsPlatformStaff();
#endif
public:
    App(int argc, char *argv[]);
    void executeOnMainThread(const std::function<void()>& callback);
    ~App();

    static App* instance();

protected:
    virtual bool event(QEvent *event) override;

};

#endif // APP_H
