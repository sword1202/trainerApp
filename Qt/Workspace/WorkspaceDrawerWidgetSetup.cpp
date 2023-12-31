#include "WorkspaceDrawerWidgetSetup.h"
#include "NvgDrawer.h"
#include "App/Fonts.h"
#include "MainController.h"
#include "Utils/QtUtils.h"
#include <QByteArray>
#include <QIcon>
#include "QDrawer.h"
#include "QtDrawerTextImagesFactory.h"
#include <QDebug>
#include <QThread>
#include <QApplication>
#include "Executors.h"
#include "QtWidgetMouseEventsReceiver.h"

using std::cout;
using std::cerr;
using std::endl;
using namespace CppUtils;

WorkspaceDrawerWidgetSetup::WorkspaceDrawerWidgetSetup() = default;

Drawer::Image *WorkspaceDrawerWidgetSetup::createImageFromSvg(const char* url, double width, double height,
        Drawer *drawer, QWidget *widget) {
    QIcon icon(url);
    qreal ratio = widget->devicePixelRatioF();
    QPixmap pixmap = icon.pixmap(qRound(width * ratio), qRound(height * ratio));

    QImage image = pixmap.toImage();
    return drawer->createImage(image.constBits(), image.width(), image.height());
}

static Drawer::Image* createImage(const QString& url, Drawer *drawer) {
    QImage image;
    image.load(url);
    image = image.rgbSwapped();
    assert(!image.isNull());
    return drawer->createImage(image.constBits(), image.width(), image.height());
}

void WorkspaceDrawerWidgetSetup::initImages(Drawer *drawer, QWidget *widget) {
    // PlayHead
    Drawer::Image* playHeadImage = createImageFromSvg(":qml/sharedimages/play_head_triangle.svg",
            WorkspaceDrawer::PLAYHEAD_TRIANGLE_WIDTH,
            WorkspaceDrawer::PLAYHEAD_TRIANGLE_HEIGHT,
            drawer, widget);
    workspaceDrawer->setPlayHeadTriangleImage(playHeadImage);

    // Clock
    Drawer::Image* clockImage = createImage(":qml/sharedimages/clock.png", drawer);
    workspaceDrawer->setClockImage(clockImage);

    // Instrumental track button
    Drawer::Image* instrumentalTrackButtonImage = createImage(":qml/sharedimages/instrumental_track_button.png",
            drawer);
    workspaceDrawer->setInstrumentalTrackButtonImage(instrumentalTrackButtonImage);

    // Vocal track button
    Drawer::Image* pianoTrackButtonImage = createImage(":qml/sharedimages/vocal_track_button.png", drawer);
    workspaceDrawer->setPianoTrackButtonImage(pianoTrackButtonImage);
}

void WorkspaceDrawerWidgetSetup::setupWorkspaceDrawer(QWidget* widget, Drawer* drawer, bool useUpdateLoop) {
    auto* factory = new QtDrawerTextImagesFactory();
    factory->load(drawer, widget->devicePixelRatio());
    drawer->setTextImagesFactory(factory);
    drawer->setTextDrawStrategy(Drawer::DRAW_USING_PRE_BUILD_IMAGES);

    workspaceDrawer = new WorkspaceDrawer(drawer, new QtWidgetMouseEventsReceiver(widget), [=] {
        onRequestUpdate(widget);
    });
    initImages(drawer, widget);

    MainController::instance()->setWorkspaceController(workspaceDrawer);
    if (useUpdateLoop) {
        // Fix strange bug, when grid is drawing with wrong alpha
        QtUtils::StartRepeatedTimer(widget, [=] {
            widget->repaint();
            return false;
        }, 1000 / 150); // 150fps

        QtUtils::StartRepeatedTimer(widget, [=] {
            if (workspaceDrawer->isRunning()) {
                widget->repaint();
            }
            return true;
        }, 1000 / 150); // 150fps

    }

    Executors::ExecuteOnMainThread([=] {
        widget->setMouseTracking(true);
    });
}

void WorkspaceDrawerWidgetSetup::handleResize(QWidget* widget, int w, int h) {
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
