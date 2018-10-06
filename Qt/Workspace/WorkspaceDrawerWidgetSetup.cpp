#include "WorkspaceDrawerWidgetSetup.h"
#include "NvgDrawer.h"
#include "../Fonts.h"
#include "MainController.h"
#include "../QtUtils/qtutils.h"
#include <QByteArray>
#include <QIcon>
#include "QDrawer.h"
#include "TextImagesFactory.h"
#include <QDebug>
#include <QThread>
#include <QApplication>

using std::cout;
using std::cerr;
using std::endl;
using namespace CppUtils;

WorkspaceDrawerWidgetSetup::WorkspaceDrawerWidgetSetup() {

}

Drawer::Image *WorkspaceDrawerWidgetSetup::createImageFromSvg(const char* url, double width, double height,
        Drawer *drawer, QWidget *widget) {
    QIcon icon(url);
    qreal ratio = widget->devicePixelRatioF();
    QPixmap pixmap = icon.pixmap(qRound(width * ratio), qRound(height * ratio));

    QImage image = pixmap.toImage();
    return drawer->createImage(image.bits(), image.width(), image.height());;
}

static Drawer::Image* createImage(const QString& url, Drawer *drawer) {
    QImage image;
    image.load(url);
    image = image.rgbSwapped();
    assert(!image.isNull());
    return drawer->createImage(image.bits(), image.width(), image.height());
}

void WorkspaceDrawerWidgetSetup::initImages(Drawer *drawer, QWidget *widget) {
    // PlayHead
    Drawer::Image* playHeadImage = createImageFromSvg(":qml/images/play_head_triangle.svg",
            WorkspaceDrawer::PLAYHEAD_TRIANGLE_WIDTH,
            WorkspaceDrawer::PLAYHEAD_TRIANGLE_HEIGHT,
            drawer, widget);
    workspaceDrawer->setPlayHeadTriangleImage(playHeadImage);

    // Clock
    Drawer::Image* clockImage = createImage(":qml/images/clock.png", drawer);
    workspaceDrawer->setClockImage(clockImage);
}

void WorkspaceDrawerWidgetSetup::setupWorkspaceDrawer(QWidget* widget, Drawer* drawer, bool useUpdateLoop) {
    assert(QApplication::instance()->thread() == QThread::currentThread());
    auto* factory = new TextImagesFactory();
    factory->load(drawer, widget->devicePixelRatio());
    drawer->setTextImagesFactory(factory);
    drawer->setTextDrawStrategy(Drawer::DRAW_USING_PRE_BUILD_IMAGES);

    workspaceDrawer = new WorkspaceDrawer(drawer, [=] {
        onRequestUpdate(widget);
    });
    initImages(drawer, widget);

    MainController::instance()->setWorkspaceController(workspaceDrawer);
    if (useUpdateLoop) {
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

    }

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

void WorkspaceDrawerWidgetSetup::onRequestUpdate(QWidget* widget) {
    widget->repaint();
}
