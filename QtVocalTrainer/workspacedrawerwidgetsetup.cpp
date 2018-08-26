#include "workspacedrawerwidgetsetup.h"
#include "NvgDrawer.h"
#include "fonts.h"
#include "MainController.h"
#include "QtUtils/qtutils.h"
#include <QByteArray>
#include <QIcon>
#include "QDrawer.h"

using std::cout;
using std::endl;
using namespace CppUtils;

WorkspaceDrawerWidgetSetup::WorkspaceDrawerWidgetSetup() {

}

void WorkspaceDrawerWidgetSetup::initPlayHeadTriangleImage(Drawer *drawer, QWidget *widget) {
    QIcon triangle(":qml/images/play_head_triangle.svg");
    qreal ratio = widget->devicePixelRatioF();
    QPixmap pixmap = triangle.pixmap(qRound(WorkspaceDrawer::PLAYHEAD_TRIANGLE_WIDTH * ratio),
            qRound(WorkspaceDrawer::PLAYHEAD_TRIANGLE_HEIGHT * ratio));

    Drawer::Image* imageHolder = nullptr;
    if (QDrawer* qDrawer = dynamic_cast<QDrawer*>(drawer)) {
        imageHolder = qDrawer->createImage(std::move(pixmap));
    }

    workspaceDrawer->setPlayHeadTriangleImage(imageHolder);
}

void WorkspaceDrawerWidgetSetup::setupWorkspaceDrawer(QWidget* widget, Drawer* drawer) {

    if (NvgDrawer* nvgDrawer = dynamic_cast<NvgDrawer*>(drawer)) {
        QByteArray latoRegular = Fonts::latoRegular();
        nvgDrawer->registerFont("Lato", latoRegular.data(), latoRegular.size());
    }

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
