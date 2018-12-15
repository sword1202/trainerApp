#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QListWidget>
#include <QGridLayout>

#include "Utils/BaseMainWindow.h"
#include "ProjectItemWidget.h"

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public BaseMainWindow
{
    Q_OBJECT
public:
    WelcomeWindow();
};

#endif // WELCOMEWINDOW_H
