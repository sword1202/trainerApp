#include "qmlworkspace.h"
#include <nanovg.h>
#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg_gl.h>

QmlWorkspace::QmlWorkspace()
{

}

void QmlWorkspace::renderBefore(const QRect &viewPort) {
    QmlOpenglItem::renderBefore(viewPort);

    if (!m_program) {
        m_program = new QOpenGLShaderProgram();
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                "attribute highp vec4 vertices;"
                "varying highp vec2 coords;"
                "void main() {"
                "    gl_Position = vertices;"
                "    coords = vertices.xy;"
                "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                "uniform lowp float t;"
                "varying highp vec2 coords;"
                "void main() {"
                "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
                "    i = smoothstep(t - 0.8, t + 0.8, i);"
                "    i = floor(i * 20.) / 20.;"
                "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
                "}");

        m_program->bindAttributeLocation("vertices", 0);
        m_program->link();

    }
//! [4] //! [5]
    m_program->bind();

    m_program->enableAttributeArray(0);

    float values[] = {
            -1, -1,
            1, -1,
            -1, 1,
            1, 1
    };
    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    m_program->setUniformValue("t", 0.5f);

    glViewport(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height());

    glDisable(GL_DEPTH_TEST);

    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program->disableAttributeArray(0);
    m_program->release();
    m_program = nullptr;
}

void QmlWorkspace::renderAfter(const QRect &viewPort) {
    QmlOpenglItem::renderAfter(viewPort);
    //renderBefore(viewPort);
}
