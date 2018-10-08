//
// Created by Semyon Tikhonenko on 10/7/18.
//

#ifndef METALTEST_METALVIEWCALLBACK_H
#define METALTEST_METALVIEWCALLBACK_H


class MetalViewCallback {
public:
    virtual void initMetal() = 0;
    virtual void renderMetal(int width, int height) = 0;
    virtual void metalResize(int width, int height) = 0;
    virtual ~MetalViewCallback() = default;
};


#endif //METALTEST_METALVIEWCALLBACK_H
