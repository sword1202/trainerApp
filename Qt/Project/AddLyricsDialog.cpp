//
// Created by Semyon Tykhonenko on 2019-06-08.
//

#include "AddLyricsDialog.h"
#include <QQuickWidget>
#include <QQmlContext>
#include "MainController.h"

AddLyricsDialog::AddLyricsDialog(QWidget *parent, QtCppBridge *cpp) :
        BaseQmlDialog(parent, cpp, "qrc:/qml/Project/AddLyricsDialog.qml") {

}

void AddLyricsDialog::add(const QString& text) {
    MvxPlayer* player = MainController::instance()->getPlayer();
    player->editLyrics([=](Lyrics *lyrics) {
        const auto &bounds = player->getBounds();
        LyricsInterval interval;
        interval.startTime = bounds.getStartSeek();
        interval.endTime = bounds.getEndSeek();
        interval.text = text.toUtf8().toStdString();
        lyrics->addLyricsInterval(0, interval);
    });
    close();
}
