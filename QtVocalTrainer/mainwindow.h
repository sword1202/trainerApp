#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basemainwindow.h"
#include <QOpenGLWidget>
#include <QQuickWidget>
#include <QSvgWidget>
#include "QtUtils/qtutils.h"
#include "qopenglworkspacewidget.h"
#include "MvxPlayer.h"

class MainWindow : public BaseMainWindow
{
    Q_OBJECT

    QWidget *workspaceView;

    QQuickItem *header;
    QQuickWidget *verticalScrollWidget;
    QQuickItem *verticalScroll;

    int getMinimumPlayHeadOffset() const;
    float getMinimumPlayHeadOffsetF() const;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    MainWindow();
    virtual ~MainWindow() override;
    void setupMenus();
    Q_INVOKABLE void setBoundsSelectionEnabled(bool enabled);

public slots:
    void onFileOpen();
    void onSelectMicrophone();
};

#endif // MAINWINDOW_H
