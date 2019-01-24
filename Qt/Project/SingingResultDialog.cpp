//
// Created by Semyon Tikhonenko on 2018-12-16.
//

#include "SingingResultDialog.h"
#include "Utils/BaseQmlWidget.h"
#include <QQmlContext>
#include <App/VxApp.h>
#include <QStandardPaths>

SingingResultDialog::SingingResultDialog(QWidget* parent) : QDialog(parent) {
    setAttribute(Qt::WA_DeleteOnClose, true);

    auto *qmlWidget = new BaseQmlWidget(this);
    QQmlContext *context = qmlWidget->rootContext();
    context->setContextProperty("self", this);
    qmlWidget->setSource(QUrl("qrc:/qml/Project/SingingResultDialog.qml"));
}

void SingingResultDialog::save() {
    MvxPlayer *player = VxApp::instance()->getPlayer();
    QString fileName = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0] + "/" +
            (player->getArtistNameUtf8()+ "_" + player->getSongTitleUtf8()).data();
    while (QFile::exists(fileName + ".rvx")) {
        fileName += "_";
    }

    fileName += ".rvx";

    MainController::instance()->saveRecordingIntoFile(fileName.toLocal8Bit().data());
}

void SingingResultDialog::again() {
    close();
    VxApp::instance()->getPlayer()->play();
}
