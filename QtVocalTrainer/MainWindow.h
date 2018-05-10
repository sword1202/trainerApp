#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QQuickView>

class MainWindow : public QQuickView
{
    Q_OBJECT
public:
    MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void resize();
};

#endif // MAINWINDOW_H
