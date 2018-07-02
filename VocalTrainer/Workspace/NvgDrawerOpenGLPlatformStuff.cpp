#include "NvgDrawer.h"
#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>

NvgDrawer::NvgDrawer() {
    ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

NvgDrawer::~NvgDrawer() {
    nvgDeleteGL2(ctx);
}