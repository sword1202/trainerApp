#include "ProjectItem.h"

#include <QHoverEvent>
#include <QFontMetrics>
#include <QPainter>
#include <QFileInfo>

ProjectItem::Data::Data(ProjectItem::Type type) : type(type) {

}

ProjectItem::ProjectItem(const ProjectItem::Data &data, QWidget *parent) : BaseQmlWidget(parent) {
    setSource(QUrl("qrc:/qml/ProjectItem.qml"));
}
