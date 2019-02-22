//
// Created by Semyon Tikhonenko on 10/25/18.
//

#ifndef VOCALTRAINER_METALNVGDRAWER_H
#define VOCALTRAINER_METALNVGDRAWER_H

#include "NvgDrawer.h"

class MetalNvgDrawer : public NvgDrawer {
public:
    explicit MetalNvgDrawer(void* layer);

protected:
    int getImageHandleFromFrameBuffer(void *frameBuffer) override;
    void *createFrameBuffer(int w, int h) override;
    void bindFrameBuffer(void *frameBuffer) override;
    void deleteFrameBuffer(void *frameBuffer) override;
public:
    ~MetalNvgDrawer() override;
    void clear() override;
};


#endif //VOCALTRAINER_METALNVGDRAWER_H
