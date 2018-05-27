#include "qmlworkspace.h"
#include "WorkspaceDrawer.h"
#include "Primitives.h"

using namespace CppUtils;

QmlWorkspace::QmlWorkspace() {

}

void QmlWorkspace::renderBefore(const QRect &viewPort, qreal devicePixelRatio) {
    QmlOpenglItem::renderBefore(viewPort, devicePixelRatio);

    glDisable(GL_DEPTH_TEST);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    workspaceDrawer.draw(viewPort.width(), viewPort.height(), (float)devicePixelRatio);
}

float QmlWorkspace::getIntervalWidth() const {
    return workspaceDrawer.getIntervalWidth();
}

void QmlWorkspace::setIntervalWidth(float intervalWidth) {
    if (Primitives::CompareFloats(workspaceDrawer.getIntervalWidth(), intervalWidth)) {
        return;
    }

    workspaceDrawer.setIntervalWidth(intervalWidth);
    update();
    emit intervalWidthChanged();
}

float QmlWorkspace::getIntervalHeight() const {
    return workspaceDrawer.getIntervalHeight();
}

void QmlWorkspace::setIntervalHeight(float intervalHeight) {
    if (Primitives::CompareFloats(workspaceDrawer.getIntervalHeight(), intervalHeight)) {
        return;
    }

    workspaceDrawer.setIntervalHeight(intervalHeight);
    update();
    emit intervalHeightChanged();
}

float QmlWorkspace::getVerticalOffset() const {
    return workspaceDrawer.getVerticalOffset();
}

void QmlWorkspace::setVerticalOffset(float verticalOffset) {
    if (Primitives::CompareFloats(workspaceDrawer.getVerticalOffset(), verticalOffset)) {
        return;
    }

    workspaceDrawer.setVerticalOffset(verticalOffset);
    update();
    emit verticalOffsetChanged();
}

float QmlWorkspace::getHorizontalOffset() const {
    return workspaceDrawer.getHorizontalOffset();
}

void QmlWorkspace::setHorizontalOffset(float horizontalOffset) {
    if (Primitives::CompareFloats(workspaceDrawer.getHorizontalOffset(), horizontalOffset)) {
        return;
    }

    workspaceDrawer.setHorizontalOffset(horizontalOffset);
    update();
    emit horizontalOffsetChanged();
}

QColor QmlWorkspace::nvgColorToQColor(const NVGcolor &nvgColor) {
    return QColor(qRound(nvgColor.r * 255.0), qRound(nvgColor.g * 255.0), qRound(nvgColor.b * 255.0), qRound(nvgColor.a * 255.0));
}

NVGcolor QmlWorkspace::qColorToNvgColor(const QColor &qColor) {
    return nvgRGBA((uchar)qColor.red(), (uchar)qColor.green(), (uchar)qColor.blue(), (uchar)qColor.alpha());
}

QColor QmlWorkspace::getGridColor() const {
    return nvgColorToQColor(workspaceDrawer.getGridColor());
}

void QmlWorkspace::setGridColor(const QColor &color) {
    workspaceDrawer.setGridColor(qColorToNvgColor(color));
    emit gridColorChanged();
}

QColor QmlWorkspace::getAccentGridColor() const {
    return nvgColorToQColor(workspaceDrawer.getAccentGridColor());
}

void QmlWorkspace::setAccentGridColor(const QColor &color) {
    workspaceDrawer.setAccentGridColor(qColorToNvgColor(color));
    emit accentGridColorChanged();
}
