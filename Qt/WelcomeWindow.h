#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QListWidget>

#include "BaseMainWindow.h"
#include "ProjectItem.h"

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public BaseMainWindow
{
    Q_OBJECT

public:
    explicit WelcomeWindow();
    ~WelcomeWindow();

    void addItems(const QStringList &names, ProjectItem::Type type);

private:
    Ui::WelcomeWindow *ui;
};

#endif // WELCOMEWINDOW_H
