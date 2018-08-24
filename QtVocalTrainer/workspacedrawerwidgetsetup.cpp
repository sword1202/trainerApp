#include "workspacedrawerwidgetsetup.h"
#include "NvgDrawer.h"
#include "fonts.h"
#include "MainController.h"
#include "qtutils.h"
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

void WorkspaceDrawerWidgetSetup::setupWorkspaceDrawer(QWidget* widget) {
    Drawer* drawer = new NvgDrawer();

    QByteArray latoRegular = Fonts::latoRegular();
    drawer->registerFont("Lato-Regular", latoRegular.data(), latoRegular.size());
    QByteArray latoBold = Fonts::latoBold();
    drawer->registerFont("Lato-Bold", latoRegular.data(), latoRegular.size());
    workspaceDrawer = new WorkspaceDrawer(drawer, [=] {
        widget->update();
    });
    workspaceDrawer->setTactNumbersFontFamily("Lato-Regular");
    initPlayHeadImage(drawer, widget);

    MainController::instance()->setWorkspaceController(workspaceDrawer);

    QtUtils::startRepeatedTimer(widget, [=] {
        if (workspaceDrawer->isRunning()) {
            widget->update();
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
