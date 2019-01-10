#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Utils/BaseMainWindow.h"
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

class ProjectWindow : public BaseMainWindow
{
    Q_OBJECT

#ifdef __APPLE__
    MetalWorkspaceWidget *workspaceWidget;
    QMacNativeWidget* verticalScrollBarNativeWrap;
#else
    OpenGLWorkspaceWidget *workspaceWidget;
#endif

    QQuickItem *headerWithSubheader;
    QQuickItem *header;
    QScrollBar *verticalScrollBar = nullptr;
    QQuickWidget *lyricsWidget;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void updateScrollBarHeightAndPosition(int windowWidth);
    void updateVerticalScrollBarValues();
    void wheelEvent(QWheelEvent *event) override;
public:
    ProjectWindow();
    void setupMenus();
    Q_INVOKABLE void setBoundsSelectionEnabled(bool enabled);
    Q_INVOKABLE void onInputVolumeChanged(float value);
    Q_INVOKABLE void onOutputVolumeChanged(float value);
    Q_INVOKABLE void onRecordingVolumeChanged(float value);
    Q_INVOKABLE void setShowLyrics(bool value);
    Q_INVOKABLE void setShowTracks(bool value);

public slots:
    void onFileOpen();
    void onSelectMicrophone();
    void onVerticalScrollBarValueChanged(int value);
};

#endif // MAINWINDOW_H
