#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BaseMainWindow.h"
#include <QOpenGLWidget>
#include <QQuickWidget>
#include <QSvgWidget>
#include "QtUtils/qtutils.h"
#include "Workspace/OpenglWorkspaceWidget.h"
#include "MvxPlayer.h"

class MainWindow : public BaseMainWindow
{
    Q_OBJECT

    QWidget *workspaceWidget;

    QQuickItem *headerWithSubheader;
    QQuickItem *header;
    QQuickWidget *verticalScrollWidget;
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
