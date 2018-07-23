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

#include "NvgDrawer.h"
#include "ConcurrentModificationAssert.h"

using namespace CppUtils;
using std::cout;
using std::endl;

constexpr int BEATS_IN_TACT = 4;
constexpr float PITCHES_GRAPH_WIDTH_IN_INTERVALS = 4.0f;
static const int PITCH_RADIUS = 3;

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
    // cout<<"fps = "<<(1.0 / frameDuration)<<"\n";
    // old logic
    if (running) {
        horizontalOffset = horizontalOffset + intervalsPerSecond * intervalWidth * frameDuration;
    }
    frameTime = now;

    drawer->beginFrame(width, height, devicePixelRatio);
    drawer->clear();

    drawVerticalGrid();
    drawHorizontalGrid();
    drawPitches();
    drawPitchesGraph();

    drawer->endFrame();
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

void WorkspaceDrawer::drawVerticalGrid() const {
    int index = 1;
    float offset = fmod(horizontalOffset, intervalWidth * BEATS_IN_TACT);
    for (float x = intervalWidth - offset; x < width + offset; x += intervalWidth, index++) {
        drawer->beginPath();
        drawer->moveTo(x * sizeMultiplier, 0);
        drawer->setStrokeWidth(sizeMultiplier);
        drawer->lineTo(x * sizeMultiplier, height * sizeMultiplier);
        bool isBeat = index % BEATS_IN_TACT != 0;
        drawer->setStrokeColor(isBeat ? gridColor : accentGridColor);
        drawer->stroke();
    }
}

void WorkspaceDrawer::drawHorizontalLine(float y, const Color& color) const {
    drawer->beginPath();
    drawer->moveTo(0, y * sizeMultiplier);
    drawer->lineTo(width * sizeMultiplier, y * sizeMultiplier);
    drawer->setStrokeWidth(sizeMultiplier);
    drawer->setStrokeColor(color);
    drawer->stroke();
}

void WorkspaceDrawer::drawHorizontalGrid() const {
    int index = 1;
    float offset = fmod(verticalOffset, intervalHeight * Pitch::PITCHES_IN_OCTAVE);
    for (float y = height - drawer->getTranslateY() - intervalHeight + offset; y > -offset; y -= intervalHeight, index++) {
        bool isOctaveBegin = index % Pitch::PITCHES_IN_OCTAVE == 0;
        drawHorizontalLine(y, isOctaveBegin ? accentGridColor : gridColor);
    }
}

void WorkspaceDrawer::drawPitch(float x, float y, float width) const {
    drawer->roundedRect(x, y, width, intervalHeight, pitchRadius);
    drawer->fill();
}

void WorkspaceDrawer::drawPitches() const {
    assert(firstPitch.isValid());
    const VxFile* vxFile = this->vxFile;
    if (!vxFile) {
        return;
    }

    if (intervalsPerSecond <= 0) {
        return;
    }

    drawer->setFillColor(pitchColor);

    double workspaceDuration = width / intervalWidth / intervalsPerSecond;
    double timeBegin = (horizontalOffset / intervalWidth) / intervalsPerSecond - getPitchGraphDuration();
    double timeEnd = timeBegin + workspaceDuration;

    vxFile->iteratePitchesInTimeRange(timeBegin, timeEnd, [&] (const VxPitch& vxPitch) {
        double pitchTimeBegin = vxFile->ticksToSeconds(vxPitch.startTickNumber);
        double pitchDuration = vxFile->ticksToSeconds(vxPitch.ticksCount);

        double x = (pitchTimeBegin - timeBegin) / workspaceDuration * width;
        double pitchWidth = pitchDuration / workspaceDuration * width;
        int distanceFromFirstPitch = getDistanceFromFirstPitch(vxPitch.pitch);
        float y = height - (distanceFromFirstPitch + 1) * intervalHeight;
        drawPitch((float)x, y, (float)pitchWidth);
    });
}

void WorkspaceDrawer::setVxFile(const VxFile* vxFile) {
    this->vxFile = vxFile;
}

void WorkspaceDrawer::drawPitchesGraph() const {
    assert(firstPitch.isValid());
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
        y = height - (distanceFromFirstPitch + pitch.getDistanceFromLowerBound() / 2.0) * intervalHeight;
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

int WorkspaceDrawer::getDistanceFromFirstPitch(const Pitch &pitch) const {
    return pitch.getPerfectFrequencyIndex() - firstPitch.getPerfectFrequencyIndex();
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
        running(false),
        frameTime(0), drawer(drawer), onUpdateRequested(onUpdateRequested) {
    setGridColor({0x8B, 0x89, 0xB6, 0x33});
    setAccentGridColor({0x8B, 0x89, 0xB6, 0x80});
    setPitchGraphColor({0xFF, 0x5E, 0x85, 0xFF});
    setPitchColor({0x6E, 0x7E, 0xC5, 0xFF});
    setPitchRadius(PITCH_RADIUS);
}

WorkspaceDrawer::~WorkspaceDrawer() {
    if (drawer) {
        delete drawer;
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
    this->firstPitch = firstPitch;
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
