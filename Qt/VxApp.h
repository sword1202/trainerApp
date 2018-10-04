#ifndef APP_H
#define APP_H

#include <QApplication>

#include <QQmlApplicationEngine>
#include "MainController.h"
#include "Player.h"

class VxApp : public QApplication, public MainController
{
    Q_OBJECT
public:
    VxApp(int &argc, char *argv[]);
    Player* getPlayer() const;
    void executeOnMainThread(const std::function<void()>& callback);
    ~VxApp();

    static VxApp* instance();

signals:
    void mainThreadCallbackPosted(QObject* callback);
};

#endif // APP_H
