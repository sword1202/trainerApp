#include "qmlworkspace.h"
#include <nanovg.h>
#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg_gl.h>

QmlWorkspace::QmlWorkspace()
{

}

void QmlWorkspace::renderBefore(const QRect &viewPort, qreal devicePixelRation) {
    QmlOpenglItem::renderBefore(viewPort, devicePixelRation);

    glDisable(GL_DEPTH_TEST);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    NVGcontext* ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgBeginFrame(ctx, viewPort.width(), viewPort.height(), (float)devicePixelRation);
    nvgBeginPath(ctx);
    nvgRect(ctx, 0, 0, 500, 500);
    nvgFillColor(ctx, nvgRGBA(255,192,0,255));
    nvgFill(ctx);
    nvgEndFrame(ctx);
}
