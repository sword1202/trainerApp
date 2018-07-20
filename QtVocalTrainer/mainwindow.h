#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QQuickWidget>
#include <QSvgWidget>
#include "QtUtils/qtutils.h"

class QmlCppBridge;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget *workspaceView;
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
    virtual ~MainWindow();
    void setPlayHeadPosition(int position) const;
    void setupMenus();

#ifdef __APPLE__
    void doMacOsPlatformStaff();
#endif
    void setupWorkspaceView();

public slots:
    void onFileOpen();

    void updatePlayheadPosition() const;
};

#endif // MAINWINDOW_H
