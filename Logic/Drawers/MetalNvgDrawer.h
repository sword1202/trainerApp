//
// Created by Semyon Tikhonenko on 10/25/18.
//

#ifndef VOCALTRAINER_METALNVGDRAWER_H
#define VOCALTRAINER_METALNVGDRAWER_H

#include "NvgDrawer.h"

class MetalNvgDrawer : public NvgDrawer {
public:
    explicit MetalNvgDrawer(void* layer);
    ~MetalNvgDrawer() override;
    void clear() override;
};


#endif //VOCALTRAINER_METALNVGDRAWER_H
