//
// Created by Semyon Tikhonenko on 10/6/18.
//

#include "QtDrawerTextImagesFactory.h"
#include "WorkspaceDrawer.h"
#include "PianoDrawer.h"
#include <QString>
#include <QImage>
#include <iostream>

constexpr int MAX_SUPPORTED_DEVICE_PIXEL_RATIO = 2;
constexpr const char* path = ":qml/sharedimages/text/";

using std::cerr;
using namespace CppUtils;

void QtDrawerTextImagesFactory::addImage(Drawer *drawer, int devicePixelRatio, int fontSize, char character,
                                         const CppUtils::Color &color, bool useCharacterCode) {
    QString fileName = path + QString::number(fontSize * devicePixelRatio) + "_";
    if (!useCharacterCode) {
        fileName += QString(character);
    } else {
        fileName += "code" + QString::number(int(character));
    }
    fileName += "_" + QString::fromStdString(color.toHexString());
    fileName += ".png";

    std::string fileNameStd = fileName.toStdString();
    QImage image(fileName);
    assert(!image.isNull());

    Drawer::Image* imageHolder = drawer->createImage(image.bits(), image.width(), image.height());

    DrawerTextImagesFactoryCharacterData data;
    data.image = imageHolder;
    data.character = character;
    data.fontSize = fontSize;
    data.color = color;
    DrawerTextImagesFactory::addImage(data);
}

void QtDrawerTextImagesFactory::load(Drawer* drawer, int devicePixelRatio) {
    if (devicePixelRatio < 1) {
        devicePixelRatio = 1;
        cerr<<"Strange device pixel ratio passed "<<devicePixelRatio;
    } else if(devicePixelRatio > MAX_SUPPORTED_DEVICE_PIXEL_RATIO) {
        devicePixelRatio = MAX_SUPPORTED_DEVICE_PIXEL_RATIO;
        cerr<<"Unsupported device pixel ratio passed "<<devicePixelRatio;
    }

    // digits
    for (int textInt = 0; textInt <= 9; ++textInt) {
        char ch = (char)textInt + '0';
        addImage(drawer, devicePixelRatio, WorkspaceDrawer::YARD_STICK_FONT_SIZE, ch,
                WorkspaceDrawer::YARD_STICK_DOT_AND_TEXT_COLOR, false);
        addImage(drawer, devicePixelRatio, PianoDrawer::FONT_SIZE, ch, PianoDrawer::PITCH_TEXT_COLOR, false);
        addImage(drawer, devicePixelRatio, PianoDrawer::FONT_SIZE, ch, PianoDrawer::SELECTED_PITCH_TEXT_COLOR, false);
    }

    // pitch names
    for (char ch = 'A'; ch <= 'G'; ch++) {
        addImage(drawer, devicePixelRatio, PianoDrawer::FONT_SIZE, ch, PianoDrawer::PITCH_TEXT_COLOR, false);
        addImage(drawer, devicePixelRatio, PianoDrawer::FONT_SIZE, ch, PianoDrawer::SELECTED_PITCH_TEXT_COLOR, false);
    }

    addImage(drawer, devicePixelRatio, WorkspaceDrawer::CLOCK_FONT_SIZE, ':',
            WorkspaceDrawer::YARD_STICK_DOT_AND_TEXT_COLOR, true);
}