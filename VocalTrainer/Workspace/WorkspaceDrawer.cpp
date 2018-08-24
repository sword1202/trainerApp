//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "WorkspaceDrawer.h"
#include <assert.h>
#include "CountAssert.h"
#include "Pitch.h"
#include "TimeUtils.h"
#include <iostream>
#include <cmath>
#include <PlayingPitchSequence.h>

#include "NvgDrawer.h"
#include "ConcurrentModificationAssert.h"

using namespace CppUtils;
using std::cout;
using std::endl;

constexpr int BEATS_IN_TACT = 4;
constexpr float PITCHES_GRAPH_WIDTH_IN_INTERVALS = 4.0f;
constexpr float YARD_STICK_DOT_Y_OFFSET = 9.75f + 1.5f;
constexpr float PIANO_WORKSPACE_VERTICAL_LINE_TOP_MARGIN = 6;
constexpr float YARD_STICK_DOT_RADIUS = 1.5f;
constexpr float PLAYBACK_BOUNDS_BOTTOM_MARGIN = 3.75f;
constexpr float PLAYBACK_BOUNDS_HEIGHT = 15.75;
static const int PITCH_RADIUS = 3;
constexpr float PLAYBACK_BOUNDS_ROUND_RECT_RADIUS = 1.5f;
constexpr float PLAYHEAD_TRIANGLE_WIDTH = 9.75f;
constexpr float PLAYHEAD_TRIANGLE_HEIGHT = 10.5f;

void WorkspaceDrawer::resize(float width, float height, float devicePixelRatio) {
    assert(devicePixelRatio > 0);
    assert(width >= 0 && height >= 0);
    this->devicePixelRatio = devicePixelRatio;
    this->width = width;
    this->height = height;
}

void WorkspaceDrawer::draw() {
    assert(drawer && "call resize before draw");

    assert(intervalWidth >= 0);
    assert(intervalHeight >= 0);
    assert(gridColor[3] > 0 && "gridColor not initialized or is completely transparent");
    assert(accentGridColor[3] > 0 && "accentGridColor not initialized or is completely transparent");

    double now = TimeUtils::NowInSeconds();
    float frameDuration = now - frameTime;
//    cout<<"fps = "<<(1.0 / frameDuration)<<"\n";
    // old logic
    if (running) {
        horizontalOffset = horizontalOffset + intervalsPerSecond * intervalWidth * frameDuration;
    }
    frameTime = now;

    drawer->beginFrame(width, height, devicePixelRatio);
    drawer->clear();

    summarizedGridHeight = std::max((float)summarizedGridHeight, getGridHeight());

    drawer->translate(PIANO_WIDTH, 0);
    drawer->translate(0, YARD_STICK_HEIGHT + 1);
    drawer->translate(0, getGridTranslation());
    drawVerticalGrid();
    drawHorizontalGrid();
    drawPitches();
    drawPitchesGraph();
    drawer->translate(0, -YARD_STICK_HEIGHT - 1 - getGridTranslation());
    drawYardStick();
    drawSecondPlayHead();
    drawer->translate(0, YARD_STICK_HEIGHT);
    drawer->translate(-PIANO_WIDTH, 0);
    drawVerticalLine(PIANO_WIDTH, borderLineColor);
    drawer->translate(PIANO_WIDTH, 0);

    drawer->translateTo(0, 0);
    drawer->setFillColor(Color::white());
    drawer->fillRect(0, 0, PIANO_WIDTH, height);
    pianoDrawer->draw(PIANO_WIDTH, height + getMaximumGridTranslation() - getGridTranslation(), devicePixelRatio);
    drawer->setFillColor(Color::white());
    drawer->fillRect(0, 0, PIANO_WIDTH, YARD_STICK_HEIGHT);
    drawHorizontalLine(YARD_STICK_HEIGHT + 0.5f, borderLineColor);

    drawer->translate(0, PIANO_WORKSPACE_VERTICAL_LINE_TOP_MARGIN);
    drawVerticalLine(PIANO_WIDTH, borderLineColor);
    drawer->translate(0, -PIANO_WORKSPACE_VERTICAL_LINE_TOP_MARGIN);

    drawer->endFrame();
}

float WorkspaceDrawer::getGridTranslation() const {
    return getMaximumGridTranslation() * verticalScrollPosition;
}

float WorkspaceDrawer::getMaximumGridTranslation() const {
    return summarizedGridHeight - getGridHeight();
}

float WorkspaceDrawer::getIntervalWidth() const {
    return intervalWidth;
}

void WorkspaceDrawer::setIntervalWidth(float intervalWidth) {
    this->intervalWidth = intervalWidth;
}

float WorkspaceDrawer::getIntervalHeight() const {
    return intervalHeight;
}

void WorkspaceDrawer::setIntervalHeight(float intervalHeight) {
    this->intervalHeight = intervalHeight;
    pianoDrawer->setIntervalHeight(intervalHeight);
}

float WorkspaceDrawer::getVerticalOffset() const {
    return verticalOffset;
}

void WorkspaceDrawer::setVerticalOffset(float verticalOffset) {
    this->verticalOffset = verticalOffset;
}

float WorkspaceDrawer::getHorizontalOffset() const {
    return horizontalOffset;
}

void WorkspaceDrawer::setHorizontalOffset(float horizontalOffset) {
    this->horizontalOffset = horizontalOffset;
}

void WorkspaceDrawer::iterateHorizontalIntervals(const std::function<void(float x, bool isBeat)> &func) const {
    int index = 1;
    float offset = fmod(horizontalOffset, intervalWidth * BEATS_IN_TACT);
    for (float x = intervalWidth - offset; x < width + offset; x += intervalWidth, index++) {
        bool isBeat = index % BEATS_IN_TACT != 0;
        func(x, isBeat);
    }
}

void WorkspaceDrawer::drawVerticalGrid() const {
    iterateHorizontalIntervals([=] (float x, bool isBeat) {
        drawVerticalLine(x, isBeat ? gridColor : accentGridColor);
    });
}

void WorkspaceDrawer::drawHorizontalLine(float y, const Color& color) const {
    drawer->beginPath();
    drawer->moveTo(0, y * sizeMultiplier);
    drawer->lineTo(width * sizeMultiplier, y * sizeMultiplier);
    drawer->setStrokeWidth(sizeMultiplier);
    drawer->setStrokeColor(color);
    drawer->stroke();
}

void WorkspaceDrawer::drawVerticalLine(float x, const WorkspaceDrawer::Color &color) const {
    drawer->beginPath();
    drawer->moveTo(x * sizeMultiplier, -drawer->getTranslateY() + height - getGridHeight());
    drawer->setStrokeWidth(sizeMultiplier);
    drawer->lineTo(x * sizeMultiplier, height * sizeMultiplier);
    drawer->setStrokeColor(color);
    drawer->stroke();
}

void WorkspaceDrawer::drawHorizontalGrid() const {
    int index = 1;
    float offset = fmod(verticalOffset, intervalHeight * Pitch::PITCHES_IN_OCTAVE);
    float gridTranslation = getGridTranslation();
    float baseHeight = getMaximumGridTranslation() - gridTranslation + getGridHeight();
    for (float y = baseHeight - intervalHeight - gridTranslation + offset;
         y > -offset - gridTranslation; y -= intervalHeight, index++) {

        bool isOctaveBegin = index % Pitch::PITCHES_IN_OCTAVE == 0;
        drawHorizontalLine(y, isOctaveBegin ? accentGridColor : gridColor);
    }
}

void WorkspaceDrawer::drawPitch(float x, float y, float width) const {
    drawer->roundedRect(x, y, width, intervalHeight, pitchRadius);
    drawer->fill();
}

void WorkspaceDrawer::drawPitches() const {
    assert(getFirstPitch().isValid());
    const VxFile* vxFile = this->vxFile;
    if (!vxFile) {
        return;
    }

    if (intervalsPerSecond <= 0) {
        return;
    }

    drawer->setFillColor(pitchColor);

    double workspaceDuration = getWorkspaceDuration();
    double workspaceSeek = getWorkspaceSeek();
    double timeBegin = workspaceSeek - getPitchGraphDuration();
    double timeEnd = timeBegin + workspaceDuration;

    float relativeHeight = getMaximumGridTranslation() - getGridTranslation() + getGridHeight();
    vxFile->iteratePitchesInTimeRange(timeBegin, timeEnd, [&] (const VxPitch& vxPitch) {
        double pitchTimeBegin = vxFile->ticksToSeconds(vxPitch.startTickNumber);
        double pitchDuration = vxFile->ticksToSeconds(vxPitch.ticksCount);

        double x = (pitchTimeBegin - timeBegin) / workspaceDuration * width;
        double pitchWidth = pitchDuration / workspaceDuration * width;
        int distanceFromFirstPitch = getDistanceFromFirstPitch(vxPitch.pitch);
        float y = relativeHeight - (distanceFromFirstPitch + 1) * intervalHeight;
        drawPitch((float)x, y, (float)pitchWidth);
    });
}

float WorkspaceDrawer::getWorkspaceSeek() const {
    return (horizontalOffset / intervalWidth) / intervalsPerSecond;
}

float WorkspaceDrawer::getWorkspaceDuration() const {
    return width / intervalWidth / intervalsPerSecond;
}

float WorkspaceDrawer::getGridHeight() const {
    return getGridHeight(height);
}

void WorkspaceDrawer::setVxFile(const VxFile* vxFile) {
    this->vxFile = vxFile;
}

void WorkspaceDrawer::drawPitchesGraph() const {
    assert(getFirstPitch().isValid());
    assert(pitchesCollector);
    assert(pitchGraphColor[3] > 0 && "pitchGraphColor not initialized or is completely transparent");

    int pitchesCount = pitchesCollector->getPitchesCount();
    if (pitchesCount <= 0) {
        return;
    }

    int i = 0;

    while (i < pitchesCount && !pitchesCollector->getPitchAt(i).isValid()) {
        i++;
    }

    drawer->beginPath();
    drawer->setStrokeWidth(sizeMultiplier);
    drawer->setStrokeColor(pitchGraphColor);

    float pitchGraphWidth = intervalWidth * PITCHES_GRAPH_WIDTH_IN_INTERVALS;
    double duration = getPitchGraphDuration();
    double now = TimeUtils::NowInSeconds();

    double x;
    double y;

    auto getXY = [&](double time, const Pitch& pitch) {
        x = (time - now + duration) / duration * pitchGraphWidth;
        float distanceFromFirstPitch = getDistanceFromFirstPitch(pitch);
        y = summarizedGridHeight - (distanceFromFirstPitch + pitch.getDistanceFromLowerBound() / 2.0) * intervalHeight;
    };

    while (i < pitchesCount) {
        Pitch pitch = pitchesCollector->getPitchAt(i);
        if (!pitch.isValid()) {
            i++;
            continue;
        }

        double time = pitchesCollector->getTimeAt(i);
        getXY(time, pitch);
        drawer->moveTo((float)x, (float)y);

        i++;
        if (i >= pitchesCount) {
            break;
        }

        if (!pitchesCollector->getPitchAt(i).isValid()) {
            drawer->lineTo((float)x, (float)y);
            continue;
        }

        for (; i < pitchesCount; i++) {
            Pitch pitch = pitchesCollector->getPitchAt(i);
            if (!pitch.isValid()) {
                break;
            }

            double time = pitchesCollector->getTimeAt(i);
            getXY(time, pitch);
            drawer->lineTo((float)x, (float)y);
        }
    }
    drawer->stroke();
}

void WorkspaceDrawer::drawYardStick() const {
    drawBoundsIfNeed();
    drawer->setFillColor(yardStickDotAndTextColor);
    int startTactIndex = (int)(getHorizontalOffset() / (getIntervalWidth() * BEATS_IN_TACT));

    drawer->setTextFont(tactNumbersFontFamily, 14);
    drawer->setTextAlign(Drawer::TextAlign::CENTER);
    drawer->setTextBaseline(Drawer::TextBaseline::MIDDLE);

    iterateHorizontalIntervals([&](float x, bool isBeat) {
        if (isBeat) {
            this->drawYardStickDot(x, YARD_STICK_DOT_Y_OFFSET);
        } else {
            if (startTactIndex > 0) {
                drawer->fillText(std::to_string(startTactIndex), x, YARD_STICK_DOT_Y_OFFSET);
            }
            startTactIndex++;
        }
    });
}

void WorkspaceDrawer::drawBoundsIfNeed() const {
    PlaybackBounds bounds = this->playbackBounds;
    float startX, width;
    if (!getBoundsStartXAndWidth(bounds, &startX, &width)) {
        return;
    }

    drawer->setFillColor(boundsColor);
    float y = YARD_STICK_HEIGHT - PLAYBACK_BOUNDS_BOTTOM_MARGIN - PLAYBACK_BOUNDS_HEIGHT;
    drawer->roundedRect(startX, y, width, PLAYBACK_BOUNDS_HEIGHT, PLAYBACK_BOUNDS_ROUND_RECT_RADIUS);
    drawer->fill();
}

void WorkspaceDrawer::drawYardStickDot(float x, float y) const {
    drawer->beginPath();
    drawer->arc(x, y, YARD_STICK_DOT_RADIUS, 0, 2 * M_PI);
    drawer->fill();
}

void WorkspaceDrawer::drawSecondPlayHead() const {
//    PlaybackBounds bounds = this->playbackBounds;
//    float startX, width;
//    if (!getBoundsStartXAndWidth(bounds, &startX, &width)) {
//        return;
//    }

//    float x = startX + width;

    drawer->setFillColor(DrawerColor::black());
    drawer->rect(0, 0, PLAYHEAD_TRIANGLE_WIDTH, playHeadImage->height());
    drawer->fillWithImage(playHeadImage);
}

int WorkspaceDrawer::getDistanceFromFirstPitch(const Pitch &pitch) const {
    return pitch.getPerfectFrequencyIndex() - firstPitchIndex;
}

double WorkspaceDrawer::getPitchGraphDuration() const {
    return getIntervalDuration() * PITCHES_GRAPH_WIDTH_IN_INTERVALS;
}

double WorkspaceDrawer::getIntervalDuration() const {
    assert(intervalsPerSecond > 0);
    return 1.0 / intervalsPerSecond;
}

const WorkspaceDrawer::Color & WorkspaceDrawer::getGridColor() const {
    return gridColor;
}

void WorkspaceDrawer::setGridColor(const Color& color) {
    // Should be called only once before rendering to avoid synchronization issues
    this->gridColor = color;
}

const WorkspaceDrawer::Color & WorkspaceDrawer::getAccentGridColor() const {
    return accentGridColor;
}

void WorkspaceDrawer::setAccentGridColor(const Color& color) {
    // Should be called only once before rendering to avoid synchronization issues
    this->accentGridColor = color;
}

WorkspaceDrawer::WorkspaceDrawer(Drawer *drawer, const std::function<void()>& onUpdateRequested) :
        intervalWidth(-1),
        intervalHeight(-1),
        verticalOffset(0),
        horizontalOffset(0),
        sizeMultiplier(1),
        intervalsPerSecond(0),
        verticalScrollPosition(0),
        summarizedGridHeight(0),
        running(false),
        firstPitchIndex(-1),
        frameTime(0), drawer(drawer), onUpdateRequested(onUpdateRequested) {
    setGridColor({0x8B, 0x89, 0xB6, 0x33});
    setAccentGridColor({0x8B, 0x89, 0xB6, 0x80});
    setPitchGraphColor({0xFF, 0x5E, 0x85, 0xFF});
    setPitchColor({0x6E, 0x7E, 0xC5, 0xFF});
    setPitchRadius(PITCH_RADIUS);
    borderLineColor = {0x8B, 0x89, 0xB6, 0xCC};
    yardStickDotAndTextColor = {0x24, 0x23, 0x2D, 0xFF};
    boundsColor = {0xC4, 0xCD, 0xFD, 0xFF};
    playHeadColor = {0x24, 0x23, 0x2D, 0xFF};

    pianoDrawer = new PianoDrawer(drawer);
}

WorkspaceDrawer::~WorkspaceDrawer() {
    if (drawer) {
        delete drawer;
    }

    if (pianoDrawer) {
        delete pianoDrawer;
    }
}

float WorkspaceDrawer::getSizeMultiplier() const {
    return sizeMultiplier;
}

void WorkspaceDrawer::setSizeMultiplier(float sizeMultiplier) {
    assert(sizeMultiplier > 0);
    this->sizeMultiplier = sizeMultiplier;
}

double WorkspaceDrawer::getIntervalsPerSecond() const {
    return intervalsPerSecond;
}

void WorkspaceDrawer::setIntervalsPerSecond(double intervalsPerSecond) {
    this->intervalsPerSecond = intervalsPerSecond;
}

PitchesCollector *WorkspaceDrawer::getPitchesCollector() const {
    return pitchesCollector;
}

void WorkspaceDrawer::setPitchesCollector(PitchesCollector *pitchesCollector) {
    this->pitchesCollector = pitchesCollector;
}

const WorkspaceDrawer::Color &WorkspaceDrawer::getPitchGraphColor() const {
    return pitchGraphColor;
}

void WorkspaceDrawer::setPitchGraphColor(const WorkspaceDrawer::Color &pitchGraphColor) {
    this->pitchGraphColor = pitchGraphColor;
}

const WorkspaceDrawer::Color &WorkspaceDrawer::getPitchColor() const {
    return pitchColor;
}

void WorkspaceDrawer::setPitchColor(const WorkspaceDrawer::Color &pitchColor) {
    this->pitchColor = pitchColor;
}

float WorkspaceDrawer::getPitchRadius() const {
    return pitchRadius;
}

void WorkspaceDrawer::setPitchRadius(float pitchRadius) {
    assert(pitchRadius >= 0);
    this->pitchRadius = pitchRadius;
}

void WorkspaceDrawer::setFirstVisiblePitch(const Pitch &firstPitch) {
    assert(firstPitch.isValid());
    this->firstPitchIndex = firstPitch.getPerfectFrequencyIndex();
    pianoDrawer->setFirstPitch(firstPitch);
}

bool WorkspaceDrawer::isRunning() const {
    return running;
}

void WorkspaceDrawer::setRunning(bool value) {
    frameTime = TimeUtils::NowInSeconds();
    running = value;
}

void WorkspaceDrawer::update() {
    onUpdateRequested();
}

void WorkspaceDrawer::setOnUpdateRequested(const std::function<void()> &onUpdateRequested) {
    this->onUpdateRequested = onUpdateRequested;
}

void WorkspaceDrawer::setTactNumbersFontFamily(const std::string &tactNumbersFontFamily) {
    this->tactNumbersFontFamily = tactNumbersFontFamily;
}

void WorkspaceDrawer::setDetectedPitch(const Pitch &detectedPitch) {
    pianoDrawer->setDetectedPitch(detectedPitch);
}

void WorkspaceDrawer::setPitchSequence(PlayingPitchSequence *pitchSequence) {
    pianoDrawer->setPitchSequence(pitchSequence);
}

Pitch WorkspaceDrawer::getFirstPitch() const {
    return Pitch::fromPerfectFrequencyIndex(firstPitchIndex);
}

float WorkspaceDrawer::getSummarizedGridHeight() const {
    return summarizedGridHeight;
}

void WorkspaceDrawer::setSummarizedGridHeight(float summarizedGridHeight) {
    this->summarizedGridHeight = summarizedGridHeight;
}

float WorkspaceDrawer::getVerticalScrollPosition() const {
    return verticalScrollPosition;
}

void WorkspaceDrawer::setVerticalScrollPosition(float verticalScrollPosition) {
    this->verticalScrollPosition = verticalScrollPosition;
}

float WorkspaceDrawer::getGridHeight(float summaryHeight) {
    return summaryHeight - YARD_STICK_HEIGHT - 1;
}

const PlaybackBounds &WorkspaceDrawer::getPlaybackBounds() const {
    return playbackBounds;
}

void WorkspaceDrawer::setPlaybackBounds(const PlaybackBounds &playbackBounds) {
    this->playbackBounds = playbackBounds;
}

float WorkspaceDrawer::durationToWidth(double duration) const {
    return static_cast<float>(duration * intervalsPerSecond * intervalWidth);
}

bool WorkspaceDrawer::getBoundsStartXAndWidth(const PlaybackBounds &bounds, float *startX, float *width) const {
    if (!bounds) {
        return false;
    }

    double workspaceDuration = getWorkspaceDuration();
    double workspaceTimeBegin = getWorkspaceSeek();
    double workspaceTimeEnd = workspaceTimeBegin + workspaceDuration;

    if (!bounds.isInside(workspaceTimeBegin) && !bounds.isInside(workspaceTimeEnd)) {
        return false;
    }

    float tactHeight = intervalWidth * BEATS_IN_TACT;
    *startX = durationToWidth(bounds.getStartSeek() - workspaceTimeBegin) + tactHeight;
    *width = durationToWidth(bounds.getEndSeek() - workspaceTimeBegin) - *startX + tactHeight;

    return true;
}

void WorkspaceDrawer::setPlayHeadImage(Drawer::Image* image) {
    assert(playHeadImage == nullptr && image != nullptr);
    playHeadImage = image;
}

void WorkspaceDrawer::getPlayHeadImageSize(float summaryHeight, float devicePixelRatio, int* outW, int* outH) {
    *outW = (int)round(PLAYHEAD_TRIANGLE_WIDTH * devicePixelRatio);
    *outH = (int)round((getGridHeight(summaryHeight) + PLAYHEAD_TRIANGLE_HEIGHT / 2.0f + 1) * devicePixelRatio);
}

void WorkspaceDrawer::onMouseMove(float x) {

}

void WorkspaceDrawer::onMouseClick(float x) {

}
