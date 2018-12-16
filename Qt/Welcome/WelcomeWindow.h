#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QListWidget>
#include <QGridLayout>

#include "Utils/BaseMainWindow.h"

class WelcomeWindow : public BaseMainWindow
{
    Q_OBJECT
public:
    WelcomeWindow();
    Q_INVOKABLE void openExistingProject();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // WELCOMEWINDOW_H
