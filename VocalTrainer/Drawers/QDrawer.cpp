#include "QDrawer.h"

static QColor toQColor(const Drawer::Color& color) {
    return QColor(color[0], color[1], color[2], color[3]);
}

QDrawer::QDrawer(QPaintDevice *paintDevice) : paintDevice(paintDevice) {
}

void QDrawer::clear() {

}

void QDrawer::translate(float x, float y) {
}

float QDrawer::getTranslateX() {
    return 0;
}

float QDrawer::getTranslateY() {
    return 0;
}

void QDrawer::translateTo(float x, float y) {

}

void QDrawer::beginFrame(float width, float height, float devicePixelRatio) {
    painter = new QPainter(paintDevice);
}

void QDrawer::endFrame() {
    delete painter;
}

void QDrawer::moveTo(float x, float y) {
    path.moveTo(x, y);
}

void QDrawer::lineTo(float x, float y) {
    path.lineTo(x, y);
}

void QDrawer::arcTo(float x1, float y1, float x2, float y2, float radius) {
    assert(false && "Not implemented");
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
    painter->strokePath(path, pen);
}

void QDrawer::fill() {
    QBrush brush;
    brush.setColor(toQColor(fillColor));
    painter->fillPath(path, brush);
}

void QDrawer::beginPath() {
    path = QPainterPath();
}

void QDrawer::closePath() {

}

void QDrawer::bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {

}

void QDrawer::quadraticCurveTo(float cpx, float cpy, float x, float y) {

}

void QDrawer::lineJoin(Drawer::LineJoin type) {

}

void QDrawer::rotate(float angle) {

}

void QDrawer::scale(float x, float y) {

}

void QDrawer::rect(float x, float y, float w, float h) {

}

void QDrawer::fillRect(float x, float y, float w, float h) {

}

void QDrawer::fillText(const std::string &text, float x, float y) {

}

QDrawer::~QDrawer() {
    assert(!painter && "endFrame not called");
}
