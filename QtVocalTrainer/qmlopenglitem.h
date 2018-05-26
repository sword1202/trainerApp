#ifndef QMLOPENGLITEM_H
#define QMLOPENGLITEM_H

#include <QQuickItem>
#include <QtGui/QOpenGLFunctions>

class QmlOpenglItem : public QQuickItem, protected QOpenGLFunctions
{
    Q_OBJECT

    QRect viewPort;
public:
    explicit QmlOpenglItem(QQuickItem *parent = nullptr);

signals:

protected slots:
    virtual void handleWindowChanged(QQuickWindow *win);

protected:
    virtual void renderBefore(const QRect& viewPort);
    virtual void renderAfter(const QRect& viewPort);
    virtual void onSync(const QQuickWindow *win);

    QRect getViewPort(const QQuickWindow *win) const;
};

#endif // QMLOPENGLITEM_H
