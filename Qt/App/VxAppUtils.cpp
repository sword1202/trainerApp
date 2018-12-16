//
// Created by Semyon Tikhonenko on 2018-12-16.
//

#include "VxAppUtils.h"
#include "VxApp.h"
#include <QString>
#include <QFileDialog>

namespace VxAppUtils {
    bool OpenExistingProject(QWidget* parent) {
        QString fileName = QFileDialog::getOpenFileName(
                parent, "Select .mvx file for signing", "", "Mvx files(*.mvx);; All files(*)");

        //QString fileName = "/Users/Semyon/Downloads/torero.mvx";
        bool fileSelected = !fileName.isEmpty();
        if (fileSelected) {
            VxApp::instance()->getPlayer()->setSource(fileName);
        }

        return fileSelected;
    }
}