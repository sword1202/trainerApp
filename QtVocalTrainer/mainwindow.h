#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QQuickWidget>
#include "workspace.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

public:
    ~MainWindow();
    Workspace *workspace;
    QQuickItem *header;

    void updateHeader();
};

#endif // MAINWINDOW_H
