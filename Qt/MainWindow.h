#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BaseMainWindow.h"
#include <QOpenGLWidget>
#include <QQuickWidget>
#include <QSvgWidget>
#include "Utils/QtUtils.h"
#include "Workspace/OpenglWorkspaceWidget.h"
#include "MvxPlayer.h"
#include <QScrollBar>
#ifdef __APPLE__
#include "Workspace/Metal/MetalWorkspaceWidget.h"
#else
#include "Workspace/OpenglWorkspaceWidget.h"
#endif

class MainWindow : public BaseMainWindow
{
    Q_OBJECT

#ifdef __APPLE__
    MetalWorkspaceWidget *workspaceWidget;
    QMacNativeWidget* virticalScrollBarNativeWrap;
#else
    QWidget *workspaceWidget;
#endif

    QQuickItem *headerWithSubheader;
    QQuickItem *header;
    QScrollBar *verticalScrollBar;
    QQuickWidget *lyricsWidget;
protected:
    void resizeEvent(QResizeEvent *event) override;
public:
    MainWindow();
    void setupMenus();
    Q_INVOKABLE void setBoundsSelectionEnabled(bool enabled);
    Q_INVOKABLE void onInputVolumeChanged(float value);
    Q_INVOKABLE void onOutputVolumeChanged(float value);
    Q_INVOKABLE void setShowLyrics(bool value);

public slots:
    void onFileOpen();
    void onSelectMicrophone();
};

#endif // MAINWINDOW_H
