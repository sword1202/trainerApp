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

int MetalNvgDrawer::getImageHandleFromFrameBuffer(void *frameBuffer) {
    return static_cast<MNVGframebuffer*>(frameBuffer)->image;
}

void *MetalNvgDrawer::createFrameBuffer(int w, int h) {
    return mnvgCreateFramebuffer(ctx, w, h, 0);
}

void MetalNvgDrawer::bindFrameBuffer(void *frameBuffer) {
    mnvgBindFramebuffer(static_cast<MNVGframebuffer*>(frameBuffer));
}

void MetalNvgDrawer::deleteFrameBuffer(void *frameBuffer) {
    mnvgDeleteFramebuffer(static_cast<MNVGframebuffer*>(frameBuffer));
}
