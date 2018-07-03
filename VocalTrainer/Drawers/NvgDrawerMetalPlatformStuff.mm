#ifdef USE_METAL

#include "NvgDrawer.h"

#include <nanovg/nanovg_mtl.h>

NvgDrawer::NvgDrawer(void* layer) {
    ctx = nvgCreateMTL(layer, NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

NvgDrawer::~NvgDrawer() {
    nvgDeleteMTL(ctx);
}

#endif