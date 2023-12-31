#include "QDrawer.h"
#include "StringUtils.h"
#include "config.h"
#include <QIcon>
#include "NotImplementedAssert.h"

using namespace CppUtils;

CPP_UTILS_DLLHIDE class QDrawerImage : public Drawer::Image {
public:
    QPixmap pixmap;

    QDrawerImage(QPixmap &&pixmap) : pixmap(std::move(pixmap)) {
    }

    int width() override {
        return pixmap.width();
    }

    int height() override {
        return pixmap.height();
    }
};

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
    assert(painter.begin(paintDevice));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
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
    NOT_IMPLEMENTED_ASSERT
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
    NOT_IMPLEMENTED_ASSERT
}

void QDrawer::quadraticCurveTo(float cpx, float cpy, float x, float y) {
    NOT_IMPLEMENTED_ASSERT
}

void QDrawer::lineJoin(Drawer::LineJoin type) {
    NOT_IMPLEMENTED_ASSERT
}

void QDrawer::rotate(float angle) {
    painter.rotate(angle);
}

void QDrawer::scale(float x, float y) {
    painter.scale(x, y);
}

void QDrawer::rect(float x, float y, float w, float h) {
    beginPath();
    path.addRect(x, y, w, h);
    closePath();
}

void QDrawer::drawTextUsingFonts(const std::string &text, float x, float y) {
    QFontMetrics fm(font);
    auto qStringText = QString::fromStdString(text);

    float width = fm.width(qStringText);
    float height = fm.capHeight();

    if (textAlign == CENTER) {
        x -= width / 2;
    } else if(textAlign == RIGHT) {
        x -= width;
    }
    if (textBaseline == MIDDLE) {
        y += height / 2;
    } else if(textBaseline == TOP) {
        y += height;
    }

    beginPath();
    path.addText(x, y, font, qStringText);
    fill();
}

void QDrawer::arc(float x, float y, float r, float sAngle, float eAngle) {
    NOT_IMPLEMENTED_ASSERT
}

void QDrawer::fillWithImage(Drawer::Image *image, float textureX1, float textureY1, float textureX2, float textureY2) {
    NOT_IMPLEMENTED_ASSERT
}

Drawer::Image *QDrawer::createImage(const void *data, int w, int h) {
    NOT_IMPLEMENTED_ASSERT
    return nullptr;
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
    beginPath();
    moveTo(x + w - radiusRightTop, y);
    if (radiusLeftTop > 0) {
        path.arcTo(x, y, radiusLeftTop, radiusLeftTop, 90.0, 90.0);
    } else {
        path.lineTo(x, y);
    }
    path.lineTo(x, y + h - radiusBottomLeft);
    if (radiusBottomLeft > 0) {
        path.arcTo(x, y + h - radiusBottomLeft, radiusBottomLeft, radiusBottomLeft, 180.0, 90.0);
    } else {
        path.lineTo(x, y + h);
    }
    path.lineTo(x + w - radiusBottomRight, y + h);
    if (radiusBottomRight > 0) {
        path.arcTo(x + w - radiusBottomRight, y + h - radiusBottomRight, radiusBottomRight,
                radiusBottomRight, 270.0, 90.0);
    } else {
        path.lineTo(x + w, y + h);
    }
    path.lineTo(x + w, y + radiusRightTop);
    if (radiusRightTop > 0) {
        path.arcTo(x + w - radiusRightTop, y, radiusRightTop,
                radiusRightTop, 0, 90.0);
    } else {
        path.lineTo(x + w, y);
    }
    closePath();
}

void QDrawer::setTextFontFamily(const char *fontFamily) {
    font.setFamily(fontFamily);
}

void QDrawer::setTextFontSize(float fontSize) {
    Drawer::setTextFontSize(fontSize);
    font.setPointSizeF(fontSize);
}

void QDrawer::setTextWeight(int weight) {
    font.setWeight(weight);
}

void QDrawer::circle(float x, float y, float r) {
    beginPath();
    path.addEllipse(QPointF(x, y), r, r);
}

Drawer::Image *QDrawer::createImage(QPixmap &&pixmap) {
    return registerImage(new QDrawerImage(std::move(pixmap)));
}

void QDrawer::drawImage(float x, float y, float w, float h, Drawer::Image *image) {
    assert(imageRegistered(image));
    painter.drawPixmap(QRectF(x, y, w, h), static_cast<QDrawerImage*>(image)->pixmap, QRectF(0, 0,
            image->width(), image->height()));
}
