#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QToolButton>
#include "Utils/BaseQmlWidget.h"

struct ProjectItem {
    enum Type {
        NEW_PROJECT,
        PROJECT,
        RECORDING
    };

    explicit ProjectItem(Type type);

    Type type;
    QString artistName;
    QString title;
    int score = 0;
};

class ProjectItemWidget : public BaseQmlWidget
{
    ProjectItem item;
public:
    ProjectItemWidget(const ProjectItem& item, QWidget *parent);
};

#endif // PROJECTITEM_H
