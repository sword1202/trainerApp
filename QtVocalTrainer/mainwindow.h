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

    QOpenGLWorkspaceWidget *workspaceView;

    QSvgWidget* playHeadTriangle;
    QFrame* playHeadLine;
    QSvgWidget* playHeadTriangle2;
    QFrame* playHeadLine2;
    std::array<int, 2> playHeadPositions;
    QQuickItem *header;
    QQuickWidget *verticalScrollWidget;
    QQuickItem *verticalScroll;
    QmlCppBridge *cpp;

    double playHeadOffsetFactor = 1.0;
    bool boundsSelectionRunning = false;

    QQuickWidget *createQQuickWidget(const QString& qmlFile);
    void movePlayHeadToPlaybackStart();
    void updatePlayHeadPosition();
    int getMinimumPlayHeadOffset() const;
    void onWorkspaceClick(QMouseEvent *event);
    void onWorkspaceMouseMove(QMouseEvent *event);
    float getMinimumPlayHeadOffsetF() const;

    void setupPlayHeadWidgets(QSvgWidget** playHeadTriangle, QFrame** playHeadLine);

    double playHeadPositionToTime(int position) const;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    void setPlayHeadPosition(int position, int index);
    int getPlayHeadPosition(int index) const;
    void setupMenus();
    Q_INVOKABLE void setBoundsSelectionEnabled(bool enabled);

#ifdef __APPLE__
    void doMacOsPlatformStaff();
#endif
    void setupWorkspaceView();

public slots:
    void onFileOpen();

    void resizePlayHeadLine(int index);
};

#endif // MAINWINDOW_H
