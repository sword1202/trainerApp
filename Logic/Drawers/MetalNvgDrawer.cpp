//
// Created by Semyon Tikhonenko on 10/25/18.
//

#include "MetalNvgDrawer.h"

#include <nanovg/nanovg_mtl.h>

void MetalNvgDrawer::clear() {
    mnvgClearWithColor(ctx, nvgRGBA(255, 255, 255, 255));
}

MetalNvgDrawer::MetalNvgDrawer(void* layer) {
    ctx = nvgCreateMTL(layer, NVG_ANTIALIAS);
}

MetalNvgDrawer::~MetalNvgDrawer() {
    nvgDeleteMTL(ctx);
}