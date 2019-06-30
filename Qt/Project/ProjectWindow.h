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

class ProjectWindow : public BaseMainWindow, CppUtils::DestructorQueue
{
    Q_OBJECT

#ifdef __APPLE__
    MetalWorkspaceWidget *workspaceWidget;
#else
    OpenGLWorkspaceWidget *workspaceWidget;
#endif

    QQuickItem *headerWithSubheader;
    QQuickItem *header;
    QQuickWidget *lyricsWidget;
    WorkspaceController* workspaceController = nullptr;
    QWidget* volumeWidget;
    bool showSaveIndicator = false;

    void setupVolumeWidget();
    void setupMenus();

protected:
    void wheelEvent(QWheelEvent *event) override;
public:
    ProjectWindow();
    Q_INVOKABLE void setBoundsSelectionEnabled(bool enabled);
    Q_INVOKABLE void onInputVolumeChanged(float value);
    Q_INVOKABLE void onOutputVolumeChanged(float value);
    Q_INVOKABLE void onRecordingVolumeChanged(float value);
    Q_INVOKABLE void setShowLyrics(bool value);
    Q_INVOKABLE void setShowTracks(bool value);

    Q_PROPERTY(qreal zoom READ getZoom() WRITE setZoom() NOTIFY zoomChanged())
    Q_PROPERTY(bool showSaveIndicator READ shouldShowSaveIndicator() NOTIFY showSaveIndicatorChanged())

    float getZoom() const;
    void setZoom(float zoom);

    bool shouldShowSaveIndicator() const;

public slots:
    void onFileOpen();
    void onSave();
    void onSelectMicrophone();
    void onAddLyrics();

signals:
    void zoomChanged();
    void showSaveIndicatorChanged();
};

#endif // MAINWINDOW_H
