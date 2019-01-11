//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#define _USE_MATH_DEFINES

#include "WorkspaceDrawer.h"
#include <assert.h>
#include "CountAssert.h"
#include "Pitch.h"
#include "TimeUtils.h"
#include <iostream>
#include <cmath>
#include <iomanip>

#include "NvgDrawer.h"
#include "Bitmap.h"
#include "AudioUtils.h"
#include "MathUtils.h"

#ifndef NDEBUG
#define CHECK_IF_RENDER_THREAD assert(checkExecutedOnRenderingThread() && "WorkspaceDrawer draw, resize and constructor should be executed  in the same thread")
#endif

using namespace CppUtils;
using std::cout;
using std::endl;

constexpr int BEATS_IN_TACT = 4;
constexpr float PITCHES_GRAPH_WIDTH_IN_INTERVALS = 4.0f;
constexpr float YARD_STICK_DOT_Y_OFFSET = 9.75f + 1.5f;
constexpr float YARD_STICK_Y_OFFSET = WorkspaceDrawer::YARD_STICK_HEIGHT - 22;
constexpr float PIANO_WORKSPACE_VERTICAL_LINE_TOP_MARGIN = 6;
constexpr float YARD_STICK_DOT_RADIUS = 1.5f;
constexpr float PLAYBACK_BOUNDS_BOTTOM_MARGIN = 3.25f;
constexpr float PLAYBACK_BOUNDS_HEIGHT = 15.75;
static const int PITCH_RADIUS = 3;
constexpr float PLAYBACK_BOUNDS_ROUND_RECT_RADIUS = 1.5f;
constexpr int INSTRUMENTAL_TRACK_HEIGHT = 24;
constexpr int MINIMUM_INSTRUMENTAL_TRACK_HEIGHT = 4;
constexpr float INSTRUMENTAL_TRACK_BOTTOM_MARGIN = 14.f;
constexpr float PIANO_TRACK_BUTTON_LEFT = 19.f;
constexpr float INSTRUMENTAL_TRACK_BUTTON_LEFT = 19.f;
constexpr float TRACK_BUTTON_HEIGHT = 17.f;
constexpr float INSTRUMENTAL_TRACK_BUTTON_WIDTH = 82.f;
constexpr float PIANO_TRACK_HEIGHT = 38.f;
constexpr float PIANO_TRACK_BOTTOM = 56.f;
constexpr float PIANO_TRACK_SHADOW_RADIUS = 50.f;
constexpr float PIANO_TRACK_SHADOW_BLUR = 25.f;
constexpr float PIANO_TRACK_PITCH_HEIGHT = 2.f;
constexpr float PIANO_TRACK_DISTANCE_BETWEEN_PITCHES = 1.f;
constexpr float PIANO_TRACK_PITCH_RADIUS = 1.f;
constexpr float PIANO_TRACK_BUTTON_WIDTH = 77.f;

constexpr int YARD_STICK_FONT_WEIGHT = 1;
static const char* FONT_FAMILY = "Lato";

#ifndef NDEBUG
bool WorkspaceDrawer::checkExecutedOnRenderingThread() {
    if (threadId == std::thread::id()) {
        threadId = std::this_thread::get_id();
    }

    return std::this_thread::get_id() == threadId;
}
#endif

void WorkspaceDrawer::resize(float width, float height, float devicePixelRatio) {
    CHECK_IF_RENDER_THREAD;
    assert(devicePixelRatio > 0);
    assert(width >= 0 && height >= 0);
    this->devicePixelRatio = devicePixelRatio;
    this->width = width;
    this->height = height;

    generateInstrumentalTrackSamplesImage(width - PIANO_WIDTH);
}

void WorkspaceDrawer::generateInstrumentalTrackSamplesImage(float width) {
    drawer->deleteImage(instrumentalTrackImage);

    if (instrumentalTrackSamples.empty()) {
        return;
    }

    int bitmapWidth = int(round(width * devicePixelRatio));
    int bitmapHeight = int(round(INSTRUMENTAL_TRACK_HEIGHT * devicePixelRatio));
    Bitmap bitmap(bitmapWidth, bitmapHeight);
    bitmap.fill(Color::transparent());

    std::vector<short> resizedSamples = AudioUtils::ResizePreviewSamples(instrumentalTrackSamples.data(),
            instrumentalTrackSamples.size(), bitmapWidth);

    // Minimum part of track line, where opacity should be applied.
    double minimumK = 0.75;
    for (int x = 0; x < bitmapWidth; ++x) {
        int middle = bitmapHeight / 2;
        int value = Math::SelectValueFromRangeProjectedInRange<int>(resizedSamples[x],
                                                                    0, std::numeric_limits<short>::max(),
                                                                    MINIMUM_INSTRUMENTAL_TRACK_HEIGHT, middle);

        for (int y = middle - value; y < middle + value; ++y) {
            int offset = abs(y - middle);
            double k = double(offset) / value;
            Color color = instrumentalTrackColor;
            if (k >= minimumK) {
                double factor = (k - minimumK) / (1.0 - minimumK);
                double opacity = 1.0 - factor;
                color = color.applyOpacity(opacity);
            }

            bitmap.setPixel(x, y, color);
        }
    }

    instrumentalTrackImage = drawer->createImage(bitmap.getData(), bitmapWidth, bitmapHeight);
}

void WorkspaceDrawer::draw() {
    CHECK_IF_RENDER_THREAD;
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

    drawer->clear();
    drawer->beginFrame(width, height, devicePixelRatio);

    summarizedGridHeight = std::max((float)summarizedGridHeight, getVisibleGridHeight());

    drawer->translate(PIANO_WIDTH, 0);
    drawer->translate(0, YARD_STICK_HEIGHT + 1);
    drawer->translate(0, getGridTranslation());
    drawVerticalGrid();
    drawHorizontalGrid();
    drawer->translate(0, -getGridTranslation());
    drawPitches();
    drawPitchesGraph();
    drawer->translate(0, -YARD_STICK_HEIGHT - 1);
    drawYardStick();
    drawer->translate(0, YARD_STICK_HEIGHT);
    drawer->translate(-PIANO_WIDTH, 0);
    drawVerticalLine(PIANO_WIDTH, borderLineColor);

    drawer->translateTo(0, 0);
    drawHorizontalLine(YARD_STICK_HEIGHT + 0.5f, borderLineColor);
    drawer->translate(PIANO_WIDTH, 0);

    if (drawTracks) {
        drawPianoTrack();
        drawInstrumentalTrack();
    }
    drawFirstPlayHead();
    drawSecondPlayHead();

    drawer->translateTo(0, 0);
    drawer->setFillColor(Color::white());
    drawer->fillRect(0, 0, PIANO_WIDTH, height);
    pianoDrawer->draw(PIANO_WIDTH, height + getMaximumGridTranslation() - getGridTranslation(), devicePixelRatio);
    drawer->setFillColor(Color::white());
    drawer->fillRect(0, 0, PIANO_WIDTH, YARD_STICK_HEIGHT);
    drawer->setStrokeColor(borderLineColor);
    // Draw border line above piano
    drawer->drawLine(0, YARD_STICK_HEIGHT + 0.5f, PIANO_WIDTH, YARD_STICK_HEIGHT + 0.5f);

    drawer->translate(0, PIANO_WORKSPACE_VERTICAL_LINE_TOP_MARGIN);
    drawVerticalLine(PIANO_WIDTH, borderLineColor);
    drawer->translate(0, -PIANO_WORKSPACE_VERTICAL_LINE_TOP_MARGIN);

    drawer->endFrame();
}

float WorkspaceDrawer::getGridTranslation() const {
    return getMaximumGridTranslation() * verticalScrollPosition;
}

float WorkspaceDrawer::getMaximumGridTranslation() const {
    return summarizedGridHeight - getVisibleGridHeight();
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
    drawer->moveTo(x * sizeMultiplier, -drawer->getTranslateY() + height - getVisibleGridHeight());
    drawer->setStrokeWidth(sizeMultiplier);
    drawer->lineTo(x * sizeMultiplier, height * sizeMultiplier);
    drawer->setStrokeColor(color);
    drawer->stroke();
}

void WorkspaceDrawer::drawHorizontalGrid() const {
    int index = 1;
    float offset = fmod(verticalOffset, intervalHeight * Pitch::PITCHES_IN_OCTAVE);
    float gridTranslation = getGridTranslation();
    float baseHeight = getMaximumGridTranslation() - gridTranslation + getVisibleGridHeight();
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
    double timeBegin = workspaceSeek - getSingingPitchGraphDuration();
    double timeEnd = timeBegin + workspaceDuration;

    float relativeHeight = getMaximumGridTranslation() - getGridTranslation() + getVisibleGridHeight();
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

void WorkspaceDrawer::drawInstrumentalTrack() {
    if (instrumentalTrackImage) {
        drawer->drawImage(0, height - INSTRUMENTAL_TRACK_BOTTOM_MARGIN - INSTRUMENTAL_TRACK_HEIGHT,
                          instrumentalTrackImage);
        drawInstrumentalTrackButton();
    }
}

void WorkspaceDrawer::drawInstrumentalTrackButton() {
    assert(instrumentalTrackButtonImage != nullptr);
    float trackMiddle = height - INSTRUMENTAL_TRACK_BOTTOM_MARGIN - INSTRUMENTAL_TRACK_HEIGHT / 2.f;
    float y = trackMiddle - TRACK_BUTTON_HEIGHT / 2;
    drawer->drawImage(INSTRUMENTAL_TRACK_BUTTON_LEFT, y,
            INSTRUMENTAL_TRACK_BUTTON_WIDTH,
            TRACK_BUTTON_HEIGHT,
            instrumentalTrackButtonImage);
}

void WorkspaceDrawer::drawPianoTrack() {
    // Draw rectangle and shadow
    float y = height - PIANO_TRACK_BOTTOM - PIANO_TRACK_HEIGHT;
    float x = 1;
    float width = this->width - drawer->getTranslateX();
    float height = PIANO_TRACK_HEIGHT;
    // make a width of shadow a bit bigger than rect width
    drawer->drawShadow(x - 200, y, width + 400, height, PIANO_TRACK_SHADOW_RADIUS,
                       PIANO_TRACK_SHADOW_BLUR, pianoTrackShadowColor);
    drawer->setFillColor(pianoTrackColor);
    drawer->fillRect(x, y, width, height);

    // Draw pitches
    const VxFile* vxFile = this->vxFile;
    int durationInTicks = vxFile->getDurationInTicks();
    float tickSize = width / durationInTicks;

    drawer->setFillColor(pianoTrackPitchesColor);
    const auto& pitches = vxFile->getPitches();
    for (const VxPitch& vxPitch : pitches) {
        float pitchX = vxPitch.startTickNumber * tickSize;
        float pitchWidth = vxPitch.ticksCount * tickSize;
        float pitchY = (Pitch::PITCHES_IN_OCTAVE - vxPitch.pitch.getPitchInOctaveIndex() - 1) *
                       (PIANO_TRACK_PITCH_HEIGHT + PIANO_TRACK_DISTANCE_BETWEEN_PITCHES) + y;
        drawer->roundedRect(pitchX, pitchY, pitchWidth, PIANO_TRACK_PITCH_HEIGHT, PIANO_TRACK_PITCH_RADIUS);
        drawer->fill();
    }

    drawPianoTrackButton();
}

void WorkspaceDrawer::drawPianoTrackButton() {
    assert(pianoTrackButtonImage != nullptr);
    float trackMiddle = height - PIANO_TRACK_BOTTOM - PIANO_TRACK_HEIGHT / 2.f;
    float y = trackMiddle - TRACK_BUTTON_HEIGHT / 2;
    drawer->drawImage(PIANO_TRACK_BUTTON_LEFT, y,
                      PIANO_TRACK_BUTTON_WIDTH,
                      TRACK_BUTTON_HEIGHT,
                      pianoTrackButtonImage);
}

float WorkspaceDrawer::getWorkspaceSeek() const {
    return (horizontalOffset / intervalWidth) / intervalsPerSecond;
}

float WorkspaceDrawer::getWorkspaceDuration() const {
    return width / intervalWidth / intervalsPerSecond;
}

float WorkspaceDrawer::getVisibleGridHeight() const {
    return getVisibleGridHeight(height);
}

float WorkspaceDrawer::getVisibleGridWidth() const {
    return width - PIANO_WIDTH - 1;
}

float WorkspaceDrawer::getSummarizedGridWidth() const {
    float intervalsCount = totalDurationInSeconds * intervalsPerSecond;
    return intervalsCount * intervalWidth;
}

void WorkspaceDrawer::setVxFile(const VxFile* vxFile) {
    this->vxFile = vxFile;
}

void WorkspaceDrawer::initGraphPitchesArrays(float workspaceSeek) {
    double pitchesGraphDrawBeginTime;
    double pitchesGraphDrawEndTime;
    if (recording) {
        double intervalDuration = getIntervalDuration();
        pitchesGraphDrawBeginTime = workspaceSeek - getSingingPitchGraphDuration() - intervalDuration;
        // Pre-draw one beat more to avoid graph interruption
        pitchesGraphDrawEndTime = getWorkspaceDuration() + pitchesGraphDrawBeginTime + intervalDuration;
    } else {
        // Pre-draw one beat more to avoid graph interruption
        double drawInterval = this->getIntervalDuration() * (BEATS_IN_TACT + 1);
        pitchesGraphDrawBeginTime = workspaceSeek - drawInterval;
        pitchesGraphDrawEndTime = workspaceSeek + 0.001;
    }

    pitchesCollection->getPitchesInTimeRange(pitchesGraphDrawBeginTime,
                                             pitchesGraphDrawEndTime,
                                             &pitchesTimes, &pitchesFrequencies);
}

void WorkspaceDrawer::drawPitchesGraph() {
    assert(getFirstPitch().isValid());
    assert(pitchesCollection);
    assert(pitchGraphColor[3] > 0 && "pitchGraphColor not initialized or is completely transparent");

    float workspaceSeek = getWorkspaceSeek();
    initGraphPitchesArrays(workspaceSeek);
    int pitchesCount = static_cast<int>(pitchesTimes.size());

    int i = 0;

    while (i < pitchesCount && !Pitch(pitchesFrequencies[i]).isValid()) {
        i++;
    }

    drawer->beginPath();
    drawer->setStrokeWidth(sizeMultiplier);
    drawer->setStrokeColor(pitchGraphColor);

    float pitchGraphWidth = intervalWidth * PITCHES_GRAPH_WIDTH_IN_INTERVALS;
    double duration = getSingingPitchGraphDuration();

    double x;
    double y;

    float relativeHeight = getMaximumGridTranslation() - getGridTranslation() + getVisibleGridHeight();
    auto getXY = [&](double time, const Pitch& pitch) {
        x = (time - workspaceSeek + duration) / duration * pitchGraphWidth;
        float distanceFromFirstPitch = getDistanceFromFirstPitch(pitch);
        y = relativeHeight - (distanceFromFirstPitch + pitch.getDistanceFromLowerBound() / 2.0) * intervalHeight;
    };

    while (i < pitchesCount) {
        Pitch pitch(pitchesFrequencies[i]);
        if (!pitch.isValid()) {
            i++;
            continue;
        }

        double time = pitchesTimes[i];
        getXY(time, pitch);
        drawer->moveTo((float)x, (float)y);

        i++;
        if (i >= pitchesCount) {
            break;
        }

        pitch = Pitch(pitchesFrequencies[i]);
        if (!pitch.isValid()) {
            drawer->lineTo((float)x, (float)y);
            continue;
        }

        for (; i < pitchesCount; i++) {
            pitch = Pitch(pitchesFrequencies[i]);
            if (!pitch.isValid()) {
                break;
            }

            time = pitchesTimes[i];
            getXY(time, pitch);
            drawer->lineTo((float)x, (float)y);
        }
    }
    drawer->stroke();
}

void WorkspaceDrawer::drawYardStick() const {
    drawer->setTextFontSize(YARD_STICK_FONT_SIZE);
    drawer->setTextAlign(Drawer::TextAlign::CENTER);
    drawer->setTextBaseline(Drawer::TextBaseline::MIDDLE);
    drawer->setTextWeight(YARD_STICK_FONT_WEIGHT);

    drawBoundsIfNeed();

    drawer->translate(0, YARD_STICK_Y_OFFSET);
    drawer->setFillColor(yardStickDotAndTextColor);
    int startTactIndex = (int)(getHorizontalOffset() / (getIntervalWidth() * BEATS_IN_TACT));

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
    drawer->translate(0, -YARD_STICK_Y_OFFSET);
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
    drawer->setFillColor(yardStickDotAndTextColor);
    drawer->circle(x, y, YARD_STICK_DOT_RADIUS);
    drawer->fill();
}

static std::string generateClockText(float seconds) {
    return TimeUtils::ConvertSecondsToFormattedTimeString("%M:%S", static_cast<int>(seconds));
}

void WorkspaceDrawer::drawPlayHead(float x, float timeInSeconds) {
    float triangleY = YARD_STICK_HEIGHT - PLAYHEAD_TRIANGLE_HEIGHT / 2 + 1;
    float triangleX = x - PLAYHEAD_TRIANGLE_WIDTH / 2;

    drawer->drawImage(triangleX, triangleY, PLAYHEAD_TRIANGLE_WIDTH, PLAYHEAD_TRIANGLE_HEIGHT, playHeadTriangleImage);
    drawer->setStrokeColor(playHeadColor);
    drawer->drawVerticalLine(x, YARD_STICK_HEIGHT, height);

    // Draw clock above playhead
    float clockX = x - CLOCK_WIDTH / 2;
    float clockY = triangleY - CLOCK_HEIGHT;
    assert(clockImage);
    drawer->drawImage(clockX, clockY, CLOCK_WIDTH, CLOCK_HEIGHT, clockImage);

    // Draw clock text
    drawer->setTextFontSize(CLOCK_FONT_SIZE);
    drawer->setTextAlign(Drawer::CENTER);
    drawer->setTextBaseline(Drawer::MIDDLE);
    float clockTextX = x;
    float clockTextY = clockY + CLOCK_HEIGHT / 2;
    std::string clockText = generateClockText(timeInSeconds);
    drawer->fillText(clockText, clockTextX, clockTextY);
}

void WorkspaceDrawer::drawSecondPlayHead() {
    PlaybackBounds bounds = this->playbackBounds;
    float startX, width;
    if (!getBoundsStartXAndWidth(bounds, &startX, &width)) {
        return;
    }

    float x = startX + width;
    secondPlayHeadPosition = x;
    drawPlayHead(x, bounds.getEndSeek());
}

void WorkspaceDrawer::drawFirstPlayHead() {
    double time = getWorkspaceSeek();
    float x = BEATS_IN_TACT * intervalWidth;
    firstPlayHeadPosition = x;
    drawPlayHead(BEATS_IN_TACT * intervalWidth, time);
}

int WorkspaceDrawer::getDistanceFromFirstPitch(const Pitch &pitch) const {
    return pitch.getPerfectFrequencyIndex() - firstPitchIndex;
}

double WorkspaceDrawer::getSingingPitchGraphDuration() const {
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
        frameTime(0),
        drawer(drawer),
        vxFile(nullptr),
        firstPlayHeadPosition(0),
        secondPlayHeadPosition(0),
        playbackBounds(PlaybackBounds()),
        drawTracks(true),
        onUpdateRequested(onUpdateRequested) {
    CHECK_IF_RENDER_THREAD;
    setGridColor({0x8B, 0x89, 0xB6, 0x33});
    setAccentGridColor({0x8B, 0x89, 0xB6, 0x80});
    setPitchGraphColor({0xFF, 0x5E, 0x85, 0xFF});
    setPitchColor({0x6E, 0x7E, 0xC5, 0xFF});
    setPitchRadius(PITCH_RADIUS);
    borderLineColor = {0x8B, 0x89, 0xB6, 0xCC};
    yardStickDotAndTextColor = {0x24, 0x23, 0x2D, 0xFF};
    boundsColor = {0xC4, 0xCD, 0xFD, 0xFF};
    playHeadColor = {0x24, 0x23, 0x2D, 0xFF};
    instrumentalTrackColor = {0x97, 0x98, 0xB5, 0xFF};
    trackButtonColor = {0x51, 0x4E, 0x64, 0xFF};
    pianoTrackColor = Color::white();
    pianoTrackShadowColor = {0xDD, 0xDB, 0xEE, 0x99};
    pianoTrackPitchesColor = {0x51, 0x4E, 0x64, 0xFF};

    pianoDrawer = new PianoDrawer(drawer);
    drawer->setTextFontFamily(FONT_FAMILY);

    pitchesTimes.reserve(5000);
    pitchesFrequencies.reserve(5000);
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

double WorkspaceDrawer::getTotalDurationInSeconds() const {
    return totalDurationInSeconds;
}

void WorkspaceDrawer::setTotalDurationInSeconds(double totalDurationInSeconds) {
    this->totalDurationInSeconds = totalDurationInSeconds;
}

void WorkspaceDrawer::setPitchesCollection(const PitchesCollection *pitchesCollection) {
    this->pitchesCollection = pitchesCollection;
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

float WorkspaceDrawer::getVisibleGridHeight(float summaryHeight) {
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

void WorkspaceDrawer::setPlayHeadTriangleImage(Drawer::Image *image) {
    playHeadTriangleImage = image;
}

float WorkspaceDrawer::getSeekFromXPositionOnWorkspace(float x) {
    x -= intervalWidth * BEATS_IN_TACT;
    x -= getGridBeginXPosition();

    float seek = x / intervalWidth / intervalsPerSecond;
    float workspaceSeek = getWorkspaceSeek();
    seek += workspaceSeek;
    return seek;
}

float WorkspaceDrawer::getGridBeginXPosition() const {
    return PIANO_WIDTH;
}

void WorkspaceDrawer::setClockImage(Drawer::Image *clockImage) {
    this->clockImage = clockImage;
}

float WorkspaceDrawer::getPlayHeadXPosition(int playHeadIndex) {
    assert(playHeadIndex == 0 || playHeadIndex == 1);
    return playHeadIndex == 0 ? firstPlayHeadPosition : secondPlayHeadPosition;
}

void WorkspaceDrawer::setRecording(bool recording) {
    this->recording = recording;
}

void WorkspaceDrawer::setInstrumentalTrackSamples(const std::vector<short> &instrumentalTrackSamples) {
    this->instrumentalTrackSamples = instrumentalTrackSamples;
    if (width > 0 && height > 0 && devicePixelRatio > 0) {
        generateInstrumentalTrackSamplesImage(width - PIANO_WIDTH);
    }
}

void WorkspaceDrawer::setInstrumentalTrackButtonImage(Drawer::Image *instrumentalTrackButtonImage) {
    this->instrumentalTrackButtonImage = instrumentalTrackButtonImage;
}

void WorkspaceDrawer::setPianoTrackButtonImage(Drawer::Image *pianoTrackButtonImage) {
    this->pianoTrackButtonImage = pianoTrackButtonImage;
}

void WorkspaceDrawer::setDrawTracks(bool value) {
    drawTracks = value;
}
