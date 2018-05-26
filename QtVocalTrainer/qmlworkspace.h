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
    virtual void renderBefore(const QRect &viewPort) override;

    void renderAfter(const QRect &viewPort) override;

private:
    QOpenGLShaderProgram *m_program = nullptr;
};

#endif // QMLWORKSPACE_H