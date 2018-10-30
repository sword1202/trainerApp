#include "ProjectItem.h"

#include <QHoverEvent>
#include <QFontMetrics>
#include <QPainter>
#include <QFileInfo>

constexpr int ICON_HEIGHT = 115;
constexpr int ICON_WIDTH = 168;
constexpr int NAME_FONT_SIZE = 10;

ProjectItem::ProjectItem(const QString &data, Type type, QWidget *parent) :
    QToolButton(parent),
    type(type)
{
    // Set icons
    switch (type) {
    case Project:
        icon = QIcon("://icons/project.svg");
        hoverIcon = QIcon("://icons/project_hover.svg");
        break;
    case Record:
        icon = QIcon("://icons/record.svg");
        hoverIcon = QIcon("://icons/record_hover.svg");
        break;
    case NewProject:
        icon = QIcon("://icons/new.svg");
        hoverIcon = QIcon("://icons/new_hover.svg");
        break;
    }

    // Enable hover mechanism
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);

    // Appearance
    setIcon(icon);
    setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT));
    setFont(QFont("Lato", NAME_FONT_SIZE, QFont::Normal));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setStyleSheet("border: none;");

    // Set label
    QFontMetrics metrics(font());
    if (type != NewProject) {
        // Read data
        QStringList fileData = QFileInfo(data).baseName().split(" – "); // Need to read data from file, temporary parse filename
        if (type == Record)
            percent = 14; // Need to read percents from file, temporary use this value

        setText(metrics.elidedText(fileData.at(0), Qt::ElideRight, ICON_WIDTH)
                + "\n"
                + metrics.elidedText(fileData.at(1), Qt::ElideRight, ICON_WIDTH));
    } else {
        setText(metrics.elidedText(data, Qt::ElideRight, ICON_WIDTH));
    }
}

// Change the icon on hover, because Qt does not support icon changing through stylesheet (https://bugreports.qt.io/browse/QTBUG-2982)
bool ProjectItem::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::HoverEnter:
        setIcon(hoverIcon);
        setStyleSheet("border: none;"
                      "color: rgb(97, 95, 151);");
        repaint();
        return true;
    case QEvent::HoverLeave:
        setIcon(icon);
        setStyleSheet("border: none;");
        repaint();
        return true;
    default:
        break;
    }

    return QWidget::event(event);
}

// Paint percents
void ProjectItem::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    // Draw percents at centre
    if (type == Record) {
        QFontMetrics metrics(font());
        QString percentText = QString::number(percent) + "%";

        int x = this->sizeHint().width() / 2 - metrics.width(percentText) / 2;
        int y = ICON_HEIGHT / 2 + metrics.height() / 2 - 7;

        QPainter painter(this);
        painter.setFont(font());
        painter.drawText(x, y, percentText);
    }
}
