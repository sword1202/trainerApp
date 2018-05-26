//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <nanovg.h>
#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg_gl.h>

#include "WorkspaceDrawer.h"

void WorkspaceDrawer::draw(int width, int height, float devicePixelRatio) {
    NVGcontext* ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgBeginFrame(ctx, width, height, devicePixelRatio);
    nvgBeginPath(ctx);
    nvgRect(ctx, 0, 0, 500, 500);
    nvgFillColor(ctx, nvgRGBA(255,192,0,255));
    nvgFill(ctx);
    nvgEndFrame(ctx);
}
