#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include "BaseMainWindow.h"

class WelcomeWindow : public BaseMainWindow
{
    Q_OBJECT
    QQuickWidget *widget;
public:
    explicit WelcomeWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // WELCOMEWINDOW_H