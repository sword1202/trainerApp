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
                parent, "Select .mvx file for signing", "", "Mvx files(*.mvx *.rvx);; All files(*)");

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

    void OpenProject(const QString& fileName) {
        QtMvxPlayer *player = VxApp::instance()->getPlayer();
        player->setSource(fileName);
        AppSettings settings;
        if (player->isRecording()) {
            settings.addRecording(fileName);
        } else {
            settings.addProject(fileName);
        }

//        LyricsLine lyricsLine;
//        LyricsInterval interval;
//        interval.startTime = 33;
//        interval.endTime = 44;
//        interval.text = "Hey torero, jizn kak mig, opiat zvuchit, trubi prizivnii zov";
//        lyricsLine.intervals.push_back(interval);
//        interval = LyricsInterval();
//        interval.startTime = 44.5;
//        interval.endTime = 53;
//        interval.text = "Hey torero, ti ili bik, kachaetsa chasha vesov";
//        lyricsLine.intervals.push_back(interval);
//
//        auto mvxFile = MvxFile::readFromFile(fileName.toLocal8Bit().data());
//        mvxFile.setLyrics(Lyrics(lyricsLine));
//        mvxFile.writeToFile(fileName.toLocal8Bit().data());
    }
}