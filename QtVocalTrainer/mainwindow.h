#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QQuickWidget>
#include "workspace.h"
#include <QSvgWidget>
#include "QtUtils/qtutils.h"

class QmlCppBridge;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Workspace *workspace;
    QSvgWidget* playHeadTriangle;
    QFrame* playHeadLine;
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
    void setPlayHeadPosition(int position) const;
    void setupMenus();

public slots:
    void onFileOpen();
};

#endif // MAINWINDOW_H
