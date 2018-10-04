#include "workspacedrawerwidgetsetup.h"
#include "NvgDrawer.h"
#include "fonts.h"
#include "MainController.h"
#include "QtUtils/qtutils.h"
#include <QByteArray>
#include <QIcon>
#include "QDrawer.h"
#include "config.h"

using std::cout;
using std::endl;
using namespace CppUtils;

CPP_UTILS_DLLHIDE class TextImagesFactory : public DrawerTextImagesFactory {
    constexpr static const char* path = ":qml/images/text/";

    void addImage(DrawerImpl* drawer, int devicePixelRatio, int fontSize, char character) {
        QString fileName = path + QString::number(fontSize * devicePixelRatio) + "_" + QString(character) + ".png";
        QImage image(fileName);
        assert(!image.isNull());

        Drawer::Image* imageHolder = nullptr;
#ifdef USE_NVG_DRAWER
        imageHolder = drawer->createImage(image.bits(), image.width(), image.height());
#else
        imageHolder = drawer->createImage(QPixmap::fromImage(image));
#endif
        DrawerTextImagesFactoryCharacterData data;
        data.image = imageHolder;
        data.character = character;
        data.fontSize = fontSize;
        DrawerTextImagesFactory::addImage(data);
    }
public:
    void load(DrawerImpl* drawer, int devicePixelRatio) {
        // digits
        for (int textInt = 0; textInt <= 9; ++textInt) {
            char ch = (char)textInt + '0';
            addImage(drawer, devicePixelRatio, WorkspaceDrawer::YARD_STICK_FONT_SIZE, ch);
            addImage(drawer, devicePixelRatio, PianoDrawer::FONT_SIZE, ch);
        }

        // pitch names
        for (char ch = 'A'; ch <= 'G'; ch++) {
            addImage(drawer, devicePixelRatio, PianoDrawer::FONT_SIZE, ch);
        }
    }
};

WorkspaceDrawerWidgetSetup::WorkspaceDrawerWidgetSetup() {

}

void WorkspaceDrawerWidgetSetup::initPlayHeadTriangleImage(DrawerImpl *drawer, QWidget *widget) {
    QIcon triangle(":qml/images/play_head_triangle.svg");
    qreal ratio = widget->devicePixelRatioF();
    QPixmap pixmap = triangle.pixmap(qRound(WorkspaceDrawer::PLAYHEAD_TRIANGLE_WIDTH * ratio),
            qRound(WorkspaceDrawer::PLAYHEAD_TRIANGLE_HEIGHT * ratio));

    Drawer::Image* imageHolder;
#ifdef USE_QT_DRAWER
    imageHolder = drawer->createImage(std::move(pixmap));
#else
    QImage image = pixmap.toImage();
    imageHolder = drawer->createImage(image.bits(), image.width(), image.height());
#endif

    workspaceDrawer->setPlayHeadTriangleImage(imageHolder);
}

void WorkspaceDrawerWidgetSetup::setupWorkspaceDrawer(QWidget* widget, DrawerImpl* drawer) {
    auto* factory = new TextImagesFactory();
    factory->load(drawer, widget->devicePixelRatio());
    drawer->setTextImagesFactory(factory);
    drawer->setTextDrawStrategy(Drawer::DRAW_USING_PRE_BUILD_IMAGES);

    workspaceDrawer = new WorkspaceDrawer(drawer, [=] {
        widget->repaint();
    });
    initPlayHeadTriangleImage(drawer, widget);

    MainController::instance()->setWorkspaceController(workspaceDrawer);

    // Fix strange bug, when grid is drawing with wrong alpha
    QtUtils::startRepeatedTimer(widget, [=] {
        widget->repaint();
        return false;
    }, 1000 / 150); // 150fps

    QtUtils::startRepeatedTimer(widget, [=] {
        if (workspaceDrawer->isRunning()) {
            widget->repaint();
        }
        return true;
    }, 1000 / 150); // 150fps

    widget->setMouseTracking(true);
}

void WorkspaceDrawerWidgetSetup::handleResize(QWidget* widget, int w, int h) {
    float gridHeight = WorkspaceDrawer::getGridHeight(h);
    MainController::instance()->getZoomController()->setWorkspaceGridHeight(gridHeight);
    workspaceDrawer->resize(w, h, (float)widget->devicePixelRatioF());
}

WorkspaceDrawerWidgetSetup::~WorkspaceDrawerWidgetSetup() {
    if (workspaceDrawer) {
        delete workspaceDrawer;
    }
}

void WorkspaceDrawerWidgetSetup::onMouseMove(float x) {
    MainController::instance()->getPlaybackBoundsSelectionController()->onWorkspaceMouseMove(x);
}

void WorkspaceDrawerWidgetSetup::onMouseClick(float x) {
    MainController::instance()->getPlaybackBoundsSelectionController()->onWorkspaceMouseClick(x);
}
