#ifndef QMLWORKSPACE_H
#define QMLWORKSPACE_H

#include <QObject>
#include "qmlopenglitem.h"
#include <QOpenGLShaderProgram>

class QmlWorkspace : public QmlOpenglItem
{
public:
    QmlWorkspace();

protected:
    virtual void renderBefore(const QRect &viewPort, qreal devicePixelRation) override;
private:
};

#endif // QMLWORKSPACE_H