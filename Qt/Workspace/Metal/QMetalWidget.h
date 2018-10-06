#ifndef QMetalWidget_H
#define QMetalWidget_H

#include <QWidget>
#include <mutex>

#ifdef __OBJC__

#import <Quartz/Quartz.h>
#import <Metal/Metal.h>

#endif

class QMetalWidget : public QWidget
{
    Q_OBJECT

public:
    QMetalWidget(QWidget *parent = 0);
    virtual ~QMetalWidget();
protected:
    void showEvent(QShowEvent *event) override;

    virtual void initMetal() = 0;
    virtual void renderMetal(int width, int height) = 0;
    virtual void metalResize(int width, int height) = 0;

protected:
    void resizeEvent(QResizeEvent *event) override;
#ifdef __OBJC__
protected:
    CAMetalLayer *getLayer() const;
    id<MTLDevice> getDevice() const;
private:
    void renderMetalInternal();

    CAMetalLayer *layer;
    id<MTLDevice> device;
    CVDisplayLinkRef displayLink;

    static CVReturn renderCallback(CVDisplayLinkRef displayLink,
            const CVTimeStamp *inNow,
            const CVTimeStamp *inOutputTime,
            CVOptionFlags flagsIn,
            CVOptionFlags *flagsOut,
            void *displayLinkContext);
#else
private:
    void* layer;
    void* device;
    void* ____;
#endif
private:
    bool sizeChanged = true;
    bool initialized = false;
    QSize _size;
    mutable std::mutex sizeMutex;
};

#endif // QMetalWidget
