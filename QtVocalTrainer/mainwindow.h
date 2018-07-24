#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QQuickWidget>
#include <QSvgWidget>
#include "QtUtils/qtutils.h"
#include "qopenglworkspacewidget.h"

class QmlCppBridge;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QOpenGLWorkspaceWidget *workspaceView;
    QSvgWidget* playHeadTriangle;
    QFrame* playHeadLine;
    QSvgWidget* playHeadTriangle2;
    QFrame* playHeadLine2;
    QQuickItem *header;
    QQuickItem *piano;
    QmlCppBridge *cpp;

    double playHeadOffsetFactor = 1.0;

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

    int getMinimumPlayHeadOffset() const;
};

#endif // MAINWINDOW_H
