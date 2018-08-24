#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QQuickWidget>
#include <QSvgWidget>
#include "QtUtils/qtutils.h"
#include "qopenglworkspacewidget.h"
#include "MvxPlayer.h"

class QmlCppBridge;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget *workspaceView;

    QQuickItem *header;
    QQuickWidget *verticalScrollWidget;
    QQuickItem *verticalScroll;
    QmlCppBridge *cpp;

    double playHeadOffsetFactor = 1.0;
    bool boundsSelectionRunning = false;

    QQuickWidget *createQQuickWidget(const QString& qmlFile);
    int getMinimumPlayHeadOffset() const;
    float getMinimumPlayHeadOffsetF() const;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    void setupMenus();
    Q_INVOKABLE void setBoundsSelectionEnabled(bool enabled);

#ifdef __APPLE__
    void doMacOsPlatformStaff();
#endif

public slots:
    void onFileOpen();
};

#endif // MAINWINDOW_H
