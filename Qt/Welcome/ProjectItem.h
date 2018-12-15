#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QToolButton>
#include "Utils/BaseQmlWidget.h"

class ProjectItem : public BaseQmlWidget
{
public:
    enum Type {
        NEW_PROJECT,
        PROJECT,
        RECORD
    };

    struct Data {
        explicit Data(Type type);

        Type type;
        QString artistName;
        QString title;
        int score = 0;
    };

    ProjectItem(const Data& data, QWidget *parent);
};

#endif // PROJECTITEM_H
