#ifndef QDRAWER_H
#define QDRAWER_H

#include "Drawer.h"
#include <QPainter>

class QDrawer : public Drawer
{
    QPaintDevice* paintDevice;
    QPainter painter;
    QPainterPath path;
    Color fillColor;
    Color strokeColor;
    float strokeWidth = 1;
public:
    QDrawer(QPaintDevice *paintDevice);
    ~QDrawer();

    void beginFrame(float width, float height, float devicePixelRatio) override;
    void endFrame() override;
    void moveTo(float x, float y) override;
    void lineTo(float x, float y) override;
    void arcTo(float x1, float y1, float x2, float y2, float radius) override;
    void setStrokeColor(const Color &color) override;
    void setFillColor(const Color &color) override;
    void setStrokeWidth(float strokeWidth) override;
    void stroke() override;

    void roundedRectDifferentCorners(float x, float y, float w, float h, float radiusLeftTop, float radiusRightTop, float radiusBottomRight, float radiusBottomLeft) override;

    void fill() override;
    void beginPath() override;
    void closePath() override;
    void bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override;
    void quadraticCurveTo(float cpx, float cpy, float x, float y) override;
    void lineJoin(LineJoin type) override;
    void rotate(float angle) override;
    void scale(float x, float y) override;
    void rect(float x, float y, float w, float h) override;

    void setTextFont(const char* fontFamily, float fontSize) override;
    void setTextAlign(TextAlign align) override;
    void setTextBaseline(TextBaseline baseline) override;
    void fillText(const std::string &text, float x, float y) override;

    void arc(float x, float y, float r, float sAngle, float eAngle) override;
    void fillWithImage(Image *image, float textureX1, float textureY1, float textureX2, float textureY2) override;
    Image *createImage(const void *data, int w, int h) override;

    void deleteImage(Image *&image) override;
    void setPaintDevice(QPaintDevice *paintDevice);

protected:
    void doTranslate(float x, float y) override;
};

#endif // QDRAWER_H