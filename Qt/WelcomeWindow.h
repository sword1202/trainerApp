#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include "BaseMainWindow.h"

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public BaseMainWindow
{
    Q_OBJECT

public:
    explicit WelcomeWindow();
    ~WelcomeWindow();

private:
    Ui::WelcomeWindow *ui;
};

#endif // WELCOMEWINDOW_H
