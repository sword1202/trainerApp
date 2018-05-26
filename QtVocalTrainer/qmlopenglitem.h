#ifndef QMLOPENGLITEM_H
#define QMLOPENGLITEM_H

#include <QQuickItem>
#include <QtGui/QOpenGLFunctions>

class QmlOpenglItem : public QQuickItem
{
    Q_OBJECT

    QRect viewPort;
    qreal devicePixelRation;
public:
    explicit QmlOpenglItem(QQuickItem *parent = nullptr);

signals:

protected slots:
    virtual void handleWindowChanged(QQuickWindow *win);

protected:
    virtual void renderBefore(const QRect& viewPort, qreal devicePixelRatio);
    virtual void renderAfter(const QRect& viewPort, qreal devicePixelRatio);
    virtual void onSync(const QQuickWindow *win);

    QRect getViewPort(const QQuickWindow *win) const;
};

#endif // QMLOPENGLITEM_H
