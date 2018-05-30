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
public:
    ~MainWindow();
    Workspace *workspace;
    QQuickItem *header;
};

#endif // MAINWINDOW_H
