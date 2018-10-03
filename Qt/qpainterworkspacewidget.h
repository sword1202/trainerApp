#ifndef QPAINTERWORKSPACEWIDGET_H
#define QPAINTERWORKSPACEWIDGET_H

#include <QWidget>
#include "workspacedrawerwidgetsetup.h"

class QPainterWorkspaceWidget : public QWidget, public WorkspaceDrawerWidgetSetup
{
    Q_OBJECT
public:
    explicit QPainterWorkspaceWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // QPAINTERWORKSPACEWIDGET_H