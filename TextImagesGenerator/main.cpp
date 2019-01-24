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
#include <iostream>
#include "StringUtils.h"
#include "Algorithms.h"

using namespace CppUtils;
using std::cout;

const char* pathToken = "qml/sharedimages/text/";
const char* path = "/Users/semyontikhonenko/Projects/VocalTrainer/Qt/qml/sharedimages/text/";
const char* qrcPath = "/Users/semyontikhonenko/Projects/VocalTrainer/Qt/qml.qrc";

std::vector<std::string> generatedLines;

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

    QByteArray line = "        <file>qml/sharedimages/text/" + fileName.toLocal8Bit() + "</file>";
    generatedLines.push_back(line.data());
    std::cout << line.data() << "\n";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir(path).removeRecursively();
    QDir(path).mkpath(".");

    for (int devicePixelRatio = 1; devicePixelRatio <= 2; ++devicePixelRatio) {
        for (int textInt = 0; textInt <= 9; ++textInt) {
            QString text = QString::number(textInt);
            drawText(WorkspaceDrawer::YARD_STICK_FONT_SIZE * devicePixelRatio, text, text, WorkspaceDrawer::YARD_STICK_DOT_AND_TEXT_COLOR);
            drawText(PianoDrawer::FONT_SIZE * devicePixelRatio, text, text, PianoDrawer::PITCH_TEXT_COLOR);
            drawText(PianoDrawer::FONT_SIZE * devicePixelRatio, text, text, PianoDrawer::SELECTED_PITCH_TEXT_COLOR);
        }

        for (char ch = 'A'; ch <= 'G'; ch++) {
            QChar text = QChar(ch);
            drawText(PianoDrawer::FONT_SIZE * devicePixelRatio, text, text, PianoDrawer::PITCH_TEXT_COLOR);
            drawText(PianoDrawer::FONT_SIZE * devicePixelRatio, text, text, PianoDrawer::SELECTED_PITCH_TEXT_COLOR);
        }

        drawText(WorkspaceDrawer::YARD_STICK_FONT_SIZE * devicePixelRatio,
                QChar(':'), "code" + QString::number(int(':')),
                WorkspaceDrawer::YARD_STICK_DOT_AND_TEXT_COLOR);
    }

    auto lines = Strings::ReadAllIntoLines(qrcPath);
    lines = Filter(lines, [&] (const std::string& value) {
        return value.find(pathToken) == std::string::npos;
    });
    
    InsertAfter(&lines, [] (const std::string& str) {
        return str.find(R"(<qresource prefix="/">)") != std::string::npos;
    }, generatedLines);

    Strings::WriteLinesToFile(qrcPath, lines);

    std::cout<<"Done\n";

    return 0;
}
