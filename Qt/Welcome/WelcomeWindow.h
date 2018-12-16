#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QListWidget>
#include <QGridLayout>

#include "Utils/BaseMainWindow.h"

class WelcomeWindow : public BaseMainWindow
{
    Q_OBJECT
    void setupProjectsList(QQmlContext *context);
    void showProjectWindow();
public:
    WelcomeWindow();
    Q_INVOKABLE void openExistingProject();
    Q_INVOKABLE void openRecentProject(const QString& filePath);
};

#endif // WELCOMEWINDOW_H
