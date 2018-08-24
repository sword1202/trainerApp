#include "QDrawer.h"

static QColor toQColor(const Drawer::Color& color) {
    return QColor(color[0], color[1], color[2], color[3]);
}

QDrawer::QDrawer(QPaintDevice *paintDevice) : paintDevice(paintDevice) {
}

void QDrawer::doTranslate(float x, float y) {
    painter.translate(x, y);
}

void QDrawer::beginFrame(float width, float height, float devicePixelRatio) {
    Drawer::beginFrame(width, height, devicePixelRatio);
    //painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    assert(painter.begin(paintDevice));
}

void QDrawer::endFrame() {
    painter.end();
}

void QDrawer::moveTo(float x, float y) {
    path.moveTo(x, y);
}

void QDrawer::lineTo(float x, float y) {
    path.lineTo(x, y);
}

void QDrawer::arcTo(float x1, float y1, float x2, float y2, float radius) {

}

void QDrawer::setStrokeColor(const Drawer::Color &color) {
    strokeColor = color;
}

void QDrawer::setFillColor(const Drawer::Color &color) {
    fillColor = color;
}

void QDrawer::setStrokeWidth(float strokeWidth) {
    this->strokeWidth = strokeWidth;
}

void QDrawer::stroke() {
    QPen pen;
    pen.setColor(toQColor(strokeColor));
    pen.setWidthF(strokeWidth);
    painter.strokePath(path, pen);
}

void QDrawer::fill() {
    QBrush brush;
    brush.setColor(toQColor(fillColor));
    brush.setStyle(Qt::SolidPattern);
    painter.fillPath(path, brush);
}

void QDrawer::beginPath() {
    path = QPainterPath();
}

void QDrawer::closePath() {
    path.closeSubpath();
}

void QDrawer::bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {

}

void QDrawer::quadraticCurveTo(float cpx, float cpy, float x, float y) {

}

void QDrawer::lineJoin(Drawer::LineJoin type) {

}

void QDrawer::rotate(float angle) {
    painter.rotate(angle);
}

void QDrawer::scale(float x, float y) {

}

void QDrawer::rect(float x, float y, float w, float h) {
//    Drawer::rect(x, y, w, h);
    beginPath();
    path.addRect(x, y, w, h);
    closePath();
}

void QDrawer::fillText(const std::string &text, float x, float y) {

}

void QDrawer::arc(float x, float y, float r, float sAngle, float eAngle) {

}

void QDrawer::fillWithImage(Drawer::Image *image, float textureX1, float textureY1, float textureX2, float textureY2) {

}

void QDrawer::registerFont(const char *name, const char *data, int dataSize) {

}

Drawer::Image *QDrawer::createImage(const void *data, int w, int h) {
    return nullptr;
}

void QDrawer::deleteImage(Drawer::Image *&image) {

}

QDrawer::~QDrawer() {

}

void QDrawer::setPaintDevice(QPaintDevice *paintDevice) {
    this->paintDevice = paintDevice;
}

void QDrawer::roundedRectDifferentCorners(float x, float y, float w, float h,
        float radiusLeftTop,
        float radiusRightTop,
        float radiusBottomRight,
        float radiusBottomLeft) {
    QPainterPath roundRectPath;
    roundRectPath.moveTo(80.0, 35.0);
    roundRectPath.arcTo(70.0, 30.0, 10.0, 10.0, 0.0, 90.0);
    roundRectPath.lineTo(25.0, 30.0);
    roundRectPath.arcTo(20.0, 30.0, 10.0, 10.0, 90.0, 90.0);
    roundRectPath.lineTo(20.0, 65.0);
    roundRectPath.arcTo(20.0, 60.0, 10.0, 10.0, 180.0, 90.0);
    roundRectPath.lineTo(75.0, 70.0);
    roundRectPath.arcTo(70.0, 60.0, 10.0, 10.0, 270.0, 90.0);
    roundRectPath.closeSubpath();
}
