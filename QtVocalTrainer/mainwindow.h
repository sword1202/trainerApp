#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QQuickWidget>
#include "workspace.h"

class QmlCppBridge;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Workspace *workspace;
    QQuickItem *header;
    QQuickItem *piano;
    QmlCppBridge *cpp;

    QQuickWidget *createQQuickWidget(const QString& qmlFile);
public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    ~MainWindow();
};

#endif // MAINWINDOW_H
