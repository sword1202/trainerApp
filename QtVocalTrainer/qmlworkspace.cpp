#include "qmlworkspace.h"
#include "WorkspaceDrawer.h"

QmlWorkspace::QmlWorkspace() {
    setFlag(ItemHasContents, true);
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
    workspaceDrawer.setIntervalWidth(intervalWidth);
    emit intervalWidthChanged();
}

float QmlWorkspace::getIntervalHeight() const {
    return workspaceDrawer.getIntervalHeight();
}

void QmlWorkspace::setIntervalHeight(float intervalHeight) {
    workspaceDrawer.setIntervalHeight(intervalHeight);
    emit intervalHeightChanged();
}

float QmlWorkspace::getVerticalOffset() const {
    return workspaceDrawer.getVerticalOffset();
}

void QmlWorkspace::setVerticalOffset(float verticalOffset) {
    workspaceDrawer.setVerticalOffset(verticalOffset);
    emit verticalOffsetChanged();
}

float QmlWorkspace::getHorizontalOffset() const {
    return workspaceDrawer.getHorizontalOffset();
}

void QmlWorkspace::setHorizontalOffset(float horizontalOffset) {
    workspaceDrawer.setHorizontalOffset(horizontalOffset);
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
