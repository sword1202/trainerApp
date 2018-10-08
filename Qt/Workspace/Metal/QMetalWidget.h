#ifndef QMetalWidget_H
#define QMetalWidget_H

#include <QWidget>
#include <mutex>
#include "MetalViewCallback.h"
#ifdef __OBJC__
#import "MetalView.h"
#endif

class QMetalWidget : public QWidget, protected MetalViewCallback
{
public:
    explicit QMetalWidget(QWidget *parent = nullptr);
    ~QMetalWidget() override;
protected:
#ifdef __OBJC__
    CAMetalLayer *getLayer() const;
#endif
    void resizeEvent(QResizeEvent *event) override;
private:
#ifdef __OBJC__
    MetalView* metalView;
#else
    void* metalView;
#endif
};

#endif // QMetalWidget
