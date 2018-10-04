#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BaseMainWindow.h"
#include <QOpenGLWidget>
#include <QQuickWidget>
#include <QSvgWidget>
#include "QtUtils/qtutils.h"
#include "QOpenglWorkspaceWidget.h"
#include "MvxPlayer.h"

class MainWindow : public BaseMainWindow
{
    Q_OBJECT

    QWidget *workspaceView;

    QQuickItem *headerWithSubheader;
    QQuickItem *header;
    QQuickWidget *verticalScrollWidget;
    QQuickWidget *lyricsWidget;

    int getMinimumPlayHeadOffset() const;
    float getMinimumPlayHeadOffsetF() const;
protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    MainWindow();
    virtual ~MainWindow() override;
    void setupMenus();
    Q_INVOKABLE void setBoundsSelectionEnabled(bool enabled);
    Q_INVOKABLE void onInputVolumeChanged(float value);
    Q_INVOKABLE void onOutputVolumeChanged(float value);

public slots:
    void onFileOpen();
    void onSelectMicrophone();
};

#endif // MAINWINDOW_H
