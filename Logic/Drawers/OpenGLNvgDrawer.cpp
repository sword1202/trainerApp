//
// Created by Semyon Tikhonenko on 10/25/18.
//

#include "OpenGLNvgDrawer.h"

#ifdef _WIN32
#define NANOVG_GL3_IMPLEMENTATION
#include <GLEW/GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#define NANOVG_GL2_IMPLEMENTATION
#endif

#include <nanovg/nanovg_gl.h>
#include <nanovg/fontstash.h>
#include <NotImplementedAssert.h>

OpenGLNvgDrawer::OpenGLNvgDrawer() {
#if defined(_WIN32) or defined(__linux__)
    GLint GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult) {
        const GLubyte *er = glewGetErrorString(GlewInitResult);
        //qDebug() << "ERROR: " << reinterpret_cast<const char *>(er);
    }
    ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

#endif

#ifdef __APPLE__
    ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif

    setupBase();
}

void OpenGLNvgDrawer::clear() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

OpenGLNvgDrawer::~OpenGLNvgDrawer() {
#if defined(_WIN32) or defined(__linux__)
    nvgDeleteGL3(ctx);
#endif

#ifdef __APPLE__
    nvgDeleteGL2(ctx);
#endif
}
