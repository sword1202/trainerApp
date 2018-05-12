#ifndef WORKSPACEDRAWER_H
#define WORKSPACEDRAWER_H

#include <QObject>
#include "qmlopenglitem.h"

class WorkspaceDrawer : public QmlOpenglItem
{
    Q_OBJECT
public:
    explicit WorkspaceDrawer(QQuickItem *parent = nullptr);

protected:
    void renderBefore(const QRect &viewPort) override;

    void renderAfter(const QRect &viewPort) override;
};

#endif // WORKSPACEDRAWER_H
