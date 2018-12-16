#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QListWidget>
#include <QGridLayout>

#include "Utils/BaseMainWindow.h"

class WelcomeWindow : public BaseMainWindow
{
    Q_OBJECT
    void setupProjectsList(QQmlContext *context);
public:
    WelcomeWindow();
    Q_INVOKABLE void openExistingProject();
};

#endif // WELCOMEWINDOW_H
