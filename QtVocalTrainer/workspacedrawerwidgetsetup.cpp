#include "workspacedrawerwidgetsetup.h"
#include "NvgDrawer.h"
#include "fonts.h"
#include "MainController.h"
#include "QtUtils/qtutils.h"
#include <QByteArray>

using std::cout;
using std::endl;
using namespace CppUtils;

WorkspaceDrawerWidgetSetup::WorkspaceDrawerWidgetSetup() {

}

void WorkspaceDrawerWidgetSetup::initPlayHeadImage(Drawer *drawer, QWidget* widget) {
    int imageWidth, imageHeight;
    WorkspaceDrawer::getPlayHeadImageSize(
            widget->height(), // summaryHeight
            widget->devicePixelRatio(), &imageWidth, &imageHeight);

    QImage image(":qml/images/playhead.png");
    image = image.scaledToWidth(imageWidth, Qt::FastTransformation);
    assert(!image.isNull());
    auto* imageHolder = drawer->createImage(image.bits(), image.width(), image.height());
    workspaceDrawer->setPlayHeadImage(imageHolder);
}

void WorkspaceDrawerWidgetSetup::setupWorkspaceDrawer(QWidget* widget, Drawer* drawer) {

    if (NvgDrawer* nvgDrawer = dynamic_cast<NvgDrawer*>(drawer)) {
        QByteArray latoRegular = Fonts::latoRegular();
        nvgDrawer->registerFont("Lato", latoRegular.data(), latoRegular.size());
    }

    workspaceDrawer = new WorkspaceDrawer(drawer, [=] {
        widget->repaint();
    });
    initPlayHeadImage(drawer, widget);

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
