#include "ProjectItemWidget.h"

#include <QHoverEvent>
#include <QFontMetrics>
#include <QPainter>
#include <QFileInfo>
#include <QQmlContext>

ProjectItem::ProjectItem(ProjectItem::Type type) : type(type) {

}

ProjectItemWidget::ProjectItemWidget(const ProjectItem &item, QWidget *parent) : BaseQmlWidget(parent), item(item) {
    setSource(QUrl("qrc:/qml/ProjectItem.qml"));

    if (item.type != ProjectItem::NEW_PROJECT) {
        auto* context = rootContext();
        context->setContextProperty("title", item.title);
        context->setContextProperty("artistName", item.artistName);
        context->setContextProperty("score", item.score);
    }
}
