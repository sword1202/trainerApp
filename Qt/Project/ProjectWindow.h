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
    QMacNativeWidget* horizontalScrollBarNativeWrap;
#else
    OpenGLWorkspaceWidget *workspaceWidget;
#endif

    QQuickItem *headerWithSubheader;
    QQuickItem *header;
    QScrollBar *verticalScrollBar = nullptr;
    QScrollBar *horizontalScrollBar = nullptr;
    QQuickWidget *lyricsWidget;
    WorkspaceController* workspaceController = nullptr;

    void updateScrollBarValues(QScrollBar* scrollBar, int pageStep, int maximum);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void updateScrollBarsSizeAndPosition(const QSize &windowSize);
    void updateVerticalScrollBarValues();
    void updateHorizontalScrollBarValues();
    void setupHorizontalScrollBar();
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
    void onHorizontalScrollBarValueChanged(int value);
};

#endif // MAINWINDOW_H
