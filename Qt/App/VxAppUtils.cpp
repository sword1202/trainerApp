//
// Created by Semyon Tikhonenko on 2018-12-16.
//

#include "VxAppUtils.h"
#include "VxApp.h"
#include "AppSettings.h"
#include <QString>
#include <QFileDialog>

namespace VxAppUtils {
    bool OpenExistingProject(QWidget* parent) {
        QString fileName = QFileDialog::getOpenFileName(
                parent, "Select .mvx file for signing", "", "Mvx files(*.mvx);; All files(*)");

        bool fileSelected = !fileName.isEmpty();
        if (fileSelected) {
            QtMvxPlayer *player = VxApp::instance()->getPlayer();
            player->setSource(fileName);
            AppSettings settings;
            if (player->isRecording()) {
                settings.addRecording(fileName);
            } else {
                settings.addProject(fileName);
            }
        }

        return fileSelected;
    }
}