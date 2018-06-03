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

#include "NvgOpenGLDrawer.h"
#include "ConcurrentModificationAssert.h"

using namespace CppUtils;
using std::cout;
using std::endl;

constexpr int BEATS_IN_TACT = 4;

constexpr float PITCHES_GRAPH_WIDTH_IN_INTERVALS = 4.0f;

void WorkspaceDrawer::resize(float width, float height, float devicePixelRatio) {
    assert(devicePixelRatio > 0);
    assert(width >= 0 && height >= 0);
    this->devicePixelRatio = devicePixelRatio;
    this->width = width;
    this->height = height;

    if (!drawer) {
        drawer = new NvgOpenGLDrawer();
    }
}

void WorkspaceDrawer::draw() {
    assert(drawer && "call resize before draw");

    assert(intervalWidth >= 0);
    assert(intervalHeight >= 0);
    assert(gridColor[3] > 0 && "gridColor not initialized or is completely transparent");
    assert(accentGridColor[3] > 0 && "accentGridColor not initialized or is completely transparent");

    static double frameTime = 0;
    double now = TimeUtils::NowInSeconds();
    if (frameTime != 0) {
        double frameDuration = now - frameTime;
        if (intervalsPerSecond != 0) {
            horizontalOffset = horizontalOffset + frameDuration * intervalsPerSecond * intervalWidth;
        }
    }
    frameTime = now;

    drawer->clear();

    drawer->beginFrame(width, height, devicePixelRatio);
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
    ConcurrentModificationAssertBegin(vxFile);
    assert(firstPitchPerfectFrequencyIndex >= 0);
    if (!vxFile) {
        return;
    }

    drawer->setFillColor(pitchColor);

    double workspaceDuration = width / intervalWidth / intervalsPerSecond;
    double timeBegin = (horizontalOffset / intervalWidth) / intervalsPerSecond;
    double timeEnd = timeBegin + workspaceDuration;

    cout<<"iteratePitchesInTimeRange begin timeBegin = "<<timeBegin<<" timeEnd = "<<timeEnd<<endl;
    vxFile->iteratePitchesInTimeRange(timeBegin, timeEnd, [&] (const VxPitch& vxPitch) {
        static int counter = 0;
        counter++;
        double pitchTimeBegin = vxFile->ticksToSeconds(vxPitch.startTickNumber);
        double pitchDuration = vxFile->ticksToSeconds(vxPitch.ticksCount);

        double x = (pitchTimeBegin - timeBegin) / workspaceDuration * width;
        double pitchWidth = pitchDuration / workspaceDuration * width;
        float y = (getDistanceFromFirstPitch(vxPitch.pitch) - 1) * intervalHeight;
        drawPitch((float)x, y, (float)pitchWidth);
        cout<<"pitchTimeBegin = "<<pitchTimeBegin<<" pitchDuration = "<<pitchDuration<<" x = "<<x<<endl;
    });
    cout<<"iteratePitchesInTimeRange end\n";
    
    assert(pitchColor[3] > 0 && "pitchColor not initialized or is completely transparent");

    ConcurrentModificationAssertEnd(vxFile);
}

void WorkspaceDrawer::setVxFile(const VxFile* vxFile) {
    if (this->vxFile) {
        delete this->vxFile;
    }

    this->vxFile = vxFile;
}

void WorkspaceDrawer::drawPitchesGraph() const {
    assert(firstPitchPerfectFrequencyIndex >= 0);
    assert(pitchesCollector);
    assert(pitchGraphColor[3] > 0 && "pitchGraphColor not initialized or is completely transparent");

    int pitchesCount = pitchesCollector->getPitchesCount();
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
    return pitch.getPerfectFrequencyIndex() - firstPitchPerfectFrequencyIndex;
}

double WorkspaceDrawer::getPitchGraphDuration() const {
    return getIntervalDuration() * PITCHES_GRAPH_WIDTH_IN_INTERVALS;
}

double WorkspaceDrawer::getIntervalDuration() const {
    return 1.0 / intervalsPerSecond;
}

const WorkspaceDrawer::Color & WorkspaceDrawer::getGridColor() const {
    return gridColor;
}

void WorkspaceDrawer::setGridColor(const Color& color) {
    // Should be called only once before rendering to avoid synchronization issues
    CountAssert(1);
    this->gridColor = color;
}

const WorkspaceDrawer::Color & WorkspaceDrawer::getAccentGridColor() const {
    return accentGridColor;
}

void WorkspaceDrawer::setAccentGridColor(const Color& color) {
    // Should be called only once before rendering to avoid synchronization issues
    CountAssert(1);
    this->accentGridColor = color;
}

WorkspaceDrawer::WorkspaceDrawer() :
        intervalWidth(-1),
        intervalHeight(-1),
        verticalOffset(0),
        horizontalOffset(0),
        sizeMultiplier(1),
        intervalsPerSecond(0),
        firstPitchPerfectFrequencyIndex(-1)
{

}

WorkspaceDrawer::~WorkspaceDrawer() {
    if (drawer) {
        delete drawer;
    }

    if (vxFile) {
        delete vxFile;
    }
}

float WorkspaceDrawer::getSizeMultiplier() const {
    return sizeMultiplier;
}

void WorkspaceDrawer::setSizeMultiplier(float sizeMultiplier) {
    assert(sizeMultiplier > 0);
    CountAssert(1);
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
    CountAssert(1);
    this->pitchesCollector = pitchesCollector;
}

const WorkspaceDrawer::Color &WorkspaceDrawer::getPitchGraphColor() const {
    return pitchGraphColor;
}

void WorkspaceDrawer::setPitchGraphColor(const WorkspaceDrawer::Color &pitchGraphColor) {
    CountAssert(1);
    this->pitchGraphColor = pitchGraphColor;
}

const WorkspaceDrawer::Color &WorkspaceDrawer::getPitchColor() const {
    return pitchColor;
}

void WorkspaceDrawer::setPitchColor(const WorkspaceDrawer::Color &pitchColor) {
    CountAssert(1);
    this->pitchColor = pitchColor;
}

float WorkspaceDrawer::getPitchRadius() const {
    return pitchRadius;
}

void WorkspaceDrawer::setPitchRadius(float pitchRadius) {
    assert(pitchRadius >= 0);
    CountAssert(1);
    this->pitchRadius = pitchRadius;
}

int WorkspaceDrawer::getFirstPitchPerfectFrequencyIndex() const {
    return firstPitchPerfectFrequencyIndex;
}

void WorkspaceDrawer::setFirstPitchPerfectFrequencyIndex(int firstPitchPerfectFrequencyIndex) {
    assert(firstPitchPerfectFrequencyIndex >= 0);
    this->firstPitchPerfectFrequencyIndex = firstPitchPerfectFrequencyIndex;
}
