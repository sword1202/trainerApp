#include <QApplication>
#include <QFontMetrics>
#include <QPainter>
#include <QPixmap>
#include <cmath>
#include <QDir>
#include <iostream>
#include <WorkspaceDrawer.h>
#include "Color.h"
#include "PianoDrawer.h"

using namespace CppUtils;

QString path = "/Users/semyontikhonenko/Projects/VocalTrainer/Qt/qml/sharedimages/text/";

static void drawText(int textSize, QString text, QString fileNameIdPart, const Color& color) {
    QFont font;
    font.setPixelSize(textSize);
    QFontMetrics metrics(font);
    int width = metrics.width(text);
    int height = metrics.height();

    QPixmap pix = QPixmap(width, height);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setPen(QPen(color.toQColor()));
    painter.setBrush(QBrush(color.toQColor()));
    painter.setFont(font);
    painter.drawText(0, 0, width, height, Qt::AlignTop | Qt::AlignLeft, text);

    QString fileName = QString::number(textSize) + "_" + fileNameIdPart + "_" +
            QString::fromStdString(color.toHexString()) + ".png";
    QString filePath = path + fileName;
    assert(pix.save(filePath));

    std::cout<<("        <file>qml/sharedimages/text/" + fileName.toLocal8Bit() + "</file>\n").data();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir(path).removeRecursively();
    QDir(path).mkpath(".");

    for (int devicePixelRatio = 1; devicePixelRatio <= 2; ++devicePixelRatio) {
        for (int textInt = 0; textInt <= 9; ++textInt) {
            QString text = QString::number(textInt);
            drawText(11 * devicePixelRatio, text, text, WorkspaceDrawer::YARD_STICK_DOT_AND_TEXT_COLOR);
            drawText(8 * devicePixelRatio, text, text, PianoDrawer::PITCH_TEXT_COLOR);
            drawText(8 * devicePixelRatio, text, text, PianoDrawer::SELECTED_PITCH_TEXT_COLOR);
        }

        for (char ch = 'A'; ch <= 'G'; ch++) {
            QChar text = QChar(ch);
            drawText(8 * devicePixelRatio, text, text, PianoDrawer::PITCH_TEXT_COLOR);
            drawText(8 * devicePixelRatio, text, text, PianoDrawer::SELECTED_PITCH_TEXT_COLOR);
        }

        drawText(11 * devicePixelRatio, QChar(':'), "code" + QString::number(int(':')),
                WorkspaceDrawer::YARD_STICK_DOT_AND_TEXT_COLOR);
    }

    std::cout<<"Done\n";

    return 0;
}
