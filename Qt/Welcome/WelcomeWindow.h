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
    explicit WelcomeWindow();
    ~WelcomeWindow();

private:
    void addItems(const QStringList &names, ProjectItem::Type type);
    void showNoItemsInfo(QListWidget *widgetList, QGridLayout *layout);

    Ui::WelcomeWindow *ui;
};

#endif // WELCOMEWINDOW_H
