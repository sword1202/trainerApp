#ifndef QMLWORKSPACE_H
#define QMLWORKSPACE_H

#include <QObject>
#include "qmlopenglitem.h"
#include "WorkspaceDrawer.h"
#include <QOpenGLShaderProgram>
#include <nanovg.h>

class QmlWorkspace : public QmlOpenglItem
{
    Q_OBJECT
    WorkspaceDrawer workspaceDrawer;
public:
    QmlWorkspace();

    Q_PROPERTY(float intervalWidth READ getIntervalWidth() WRITE setIntervalWidth() NOTIFY intervalWidthChanged())
    Q_PROPERTY(float intervalHeight READ getIntervalHeight() WRITE setIntervalHeight() NOTIFY intervalHeightChanged())
    Q_PROPERTY(float verticalOffset READ getVerticalOffset() WRITE setVerticalOffset() NOTIFY verticalOffsetChanged())
    Q_PROPERTY(float horizontalOffset READ getHorizontalOffset() WRITE setHorizontalOffset() NOTIFY horizontalOffsetChanged())
    Q_PROPERTY(QColor gridColor READ getGridColor() WRITE setGridColor() NOTIFY gridColorChanged())
    Q_PROPERTY(QColor accentGridColor READ getAccentGridColor() WRITE setAccentGridColor() NOTIFY accentGridColorChanged())

    float getIntervalWidth() const;
    void setIntervalWidth(float intervalWidth);
    float getIntervalHeight() const;
    void setIntervalHeight(float intervalHeight);
    float getVerticalOffset() const;
    void setVerticalOffset(float verticalOffset);
    float getHorizontalOffset() const;
    void setHorizontalOffset(float horizontalOffset);

    QColor getGridColor() const;
    void setGridColor(const QColor& color);
    QColor getAccentGridColor() const;
    void setAccentGridColor(const QColor& color);

signals:
    void intervalWidthChanged();
    void intervalHeightChanged();
    void horizontalOffsetChanged();
    void verticalOffsetChanged();
    void gridColorChanged();
    void accentGridColorChanged();

protected:
    virtual void renderBefore(const QRect &viewPort, qreal devicePixelRatio) override;
private:
    static QColor nvgColorToQColor(const NVGcolor& nvgColor);
    static NVGcolor qColorToNvgColor(const QColor& qColor);
};

#endif // QMLWORKSPACE_H
