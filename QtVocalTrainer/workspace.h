#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QOpenGLWidget>
#include "WorkspaceDrawer.h"
#include "SynchronizedCallbacksQueue.h"
#include "PitchInputReaderCollector.h"

class Workspace : public QOpenGLWidget
{
    Q_OBJECT

    CppUtils::SynchronizedCallbacksQueue renderingQueue;
public:
    Workspace(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    WorkspaceDrawer workspaceDrawer;
    float devicePixelRatio_;

private slots:
    void zoomChanged();
    void isPlayingChanged(bool isPlaying);
    void playerSourceChanged();
    void firstPitchChanged();
    void seekChanged(double seek);
};

#endif // WORKSPACE_H
