#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QToolButton>

class ProjectItem : public QToolButton
{
public:
    enum Type {
        NEW_PROJECT,
        PROJECT,
        RECORD
    };

    ProjectItem(const QString &data, Type type, QWidget *parent = nullptr);

private:
    void setup();
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    int percent;
    Type type;
    QIcon icon;
    QIcon hoverIcon;
};

#endif // PROJECTITEM_H
