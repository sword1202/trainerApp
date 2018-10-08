//
// Created by Semyon Tikhonenko on 10/6/18.
//

#ifndef METALTEST_METALTESTWIDGET_H
#define METALTEST_METALTESTWIDGET_H

#include "../Qt/Workspace/Metal/QMetalWidget.h"
#include "../Logic/Drawers/NvgDrawer.h"

class MetalTestWidget : public QMetalWidget {
    NvgDrawer* drawer;
protected:
    void initMetal() override;
    void renderMetal(int width, int height) override;
    void metalResize(int width, int height) override;
public:
    explicit MetalTestWidget(QWidget* parent = nullptr);
};


#endif //METALTEST_METALTESTWIDGET_H
