//
// Created by Semyon Tikhonenko on 2018-12-16.
//

#ifndef VOCALTRAINER_VXAPPUTILS_H
#define VOCALTRAINER_VXAPPUTILS_H


#include <QWidget>

namespace VxAppUtils {
    bool OpenExistingProject(QWidget* parent);
    void OpenProject(const QString& fileName);
}


#endif //VOCALTRAINER_VXAPPUTILS_H
