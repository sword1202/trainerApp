//
// Created by Semyon Tikhonenko on 1/12/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#include "GLSceneDrawer.h"
#include "TimeUtils.h"
#include <GLUT/GLUT.h>
#include <iostream>

using std::cerr;
using std::endl;

static const int64_t DISPLAY_PITCH_TIME_LIMIT = 5000000; // in microseconds
static const float PITCH_UNIT = 2.0f / Pitch::PITCHES_IN_OCTAVE;
static const float PIANO_WIDTH = 0.2f;
static const float BLACK_WHITE_PIANO_BUTTON_RELATION = 85.0f / 145.0f;

static const float BLACK_POINTS[5][2] {
        {0.20186335403726707f, 0.3136645962732919f},
        {0.5434782608695652f, 0.65527950310559f},
        {1.0590062111801242f, 1.170807453416149f},
        {1.372670807453416f, 1.4844720496894408f},
        {1.6863354037267078f, 1.7981366459627326f}
};

static const float DIVIDER_OFFSET = 23.0f / 161.0f * 2;

void GLSceneDrawer::draw(int width, int height) {
    int64_t now = TimeUtils::nowInMicroseconds();
    auto iter = std::find_if(detectedPitches.begin(), detectedPitches.end(),
            [=](const SingerPitchDetection &pitchDetection) {
                return now - pitchDetection.time <= DISPLAY_PITCH_TIME_LIMIT;
            });

    detectedPitches.erase(detectedPitches.begin(), iter);

    glClear(GLenum(GL_COLOR_BUFFER_BIT));
    glViewport(0, 0, width, height);
    drawPitchesGraph(now);
    drawWavPitches(now);
    drawDividers();
    drawPiano();

    glFlush();
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
        cerr<<"Gl error: "<<glError<<endl;
    }
}

class PitchSelectionDrawer {
    const GLSceneDrawer* out;
    float selectedPitchX1 = -1.0f;
    float selectedPitchX2, selectedPitchY1, selectedPitchY2;

    enum DrawType {
        WHITE, BLACK, NONE
    };

    DrawType drawType = NONE;
public:
    PitchSelectionDrawer(const GLSceneDrawer *out) : out(out) {
    }

    void setup() {
        const auto& detectedPitches = out->getDetectedPitches();
        if (!detectedPitches.empty()) {
            const GLSceneDrawer::SingerPitchDetection& lastPitchDetection = detectedPitches.back();
            const Pitch& pitch = lastPitchDetection.pitch;
            int pitchInOctaveIndex = pitch.getPitchInOctaveIndex();
            if (pitchInOctaveIndex >= 0) {
                if (Pitch::isWhite(pitchInOctaveIndex)) {
                    selectedPitchX2 = -1.0f + PIANO_WIDTH;
                    int whitePitchIndex = Pitch::getWhitePitchInOctaveIndex(pitchInOctaveIndex);
                    selectedPitchY1 = whitePitchIndex * DIVIDER_OFFSET - 1.0f;
                    selectedPitchY2 = selectedPitchY1 + DIVIDER_OFFSET;
                    drawType = WHITE;
                } else {
                    selectedPitchX2 = -1.0f + PIANO_WIDTH * BLACK_WHITE_PIANO_BUTTON_RELATION;
                    int blackPointIndex = Pitch::getBlackPitchInOctaveIndex(pitchInOctaveIndex);

                    const float* blackPoint = BLACK_POINTS[blackPointIndex];
                    selectedPitchY1 = blackPoint[0] - 1.0f;
                    selectedPitchY2 = blackPoint[1] - 1.0f;
                    drawType = BLACK;
                }
            }
        }
    }

    void draw() {
        glColor3f(161 / 255.0f, 204 / 255.0f , 157 / 255.0f);
        glBegin(GL_POLYGON);
        {
            glVertex2f(selectedPitchX1, selectedPitchY1);
            glVertex2f(selectedPitchX2, selectedPitchY1);
            glVertex2f(selectedPitchX2, selectedPitchY2);
            glVertex2f(selectedPitchX1, selectedPitchY2);
        }
        glEnd();
    }

    bool shouldDrawWhite() const {
        return drawType == WHITE;
    }

    bool shouldDrawBlack() const {
        return drawType == BLACK;
    }
};

void GLSceneDrawer::drawPiano() {
    glColor3f(0.9, 0.9, 0.9);
    float x1 = -1.0f;
    float x2 = -1.0f + PIANO_WIDTH;
    float y1 = -1.0f;
    float y2 = 1.0f;
    glBegin(GL_POLYGON);
    {
        // draw white canvas
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    }
    glEnd();

    PitchSelectionDrawer selectionDrawer(this);
    selectionDrawer.setup();

    if (selectionDrawer.shouldDrawWhite()) {
        selectionDrawer.draw();
    }

    // draw piano buttons dividers
    glColor3f(0.5, 0.5, 0.5);
    int dividersCount = 6;
    float dividerY1 = DIVIDER_OFFSET - 1.0f;
    for (int i = 0; i < dividersCount; i++) {
        float dividerX1 = -1.0f;
        float dividerX2 = -1.0f + PIANO_WIDTH;
        float dividerY2 = dividerY1;

        glBegin(GL_LINES);
        {
            glVertex2f(dividerX1, dividerY1);
            glVertex2f(dividerX2, dividerY2);
        }
        glEnd();

        dividerY1 += DIVIDER_OFFSET;
    }

    // draw black piano buttons
    glColor3f(0.1, 0.1, 0.1);
    for (const float* point : BLACK_POINTS) {
        float blackY1 = point[0] - 1.0f;
        float blackY2 = point[1] - 1.0f;
        float blackX2 = -1.0f + PIANO_WIDTH * BLACK_WHITE_PIANO_BUTTON_RELATION;


        glBegin(GL_POLYGON);
        {
            glVertex2f(x1, blackY1);
            glVertex2f(blackX2, blackY1);
            glVertex2f(blackX2, blackY2);
            glVertex2f(x1, blackY2);
        }
        glEnd();
    }

    if (selectionDrawer.shouldDrawBlack()) {
        selectionDrawer.draw();
    }
}

static double getPitchPosition(const Pitch& pitch) {
    int pitchIndexInOctave = pitch.getPitchInOctaveIndex();
    float lowerBoundPosition = PITCH_UNIT * pitchIndexInOctave;
    return (lowerBoundPosition + pitch.getDistanceFromLowerBound() / 2 * PITCH_UNIT) - 1.0f;
}

static void getPitchLowerAndUpperBoundsPositions(const Pitch& pitch, float *outLower, float *outUpper) {
    int pitchIndexInOctave = pitch.getPitchInOctaveIndex();
    *outLower = PITCH_UNIT * pitchIndexInOctave - 1.0f;
    *outUpper = *outLower + PITCH_UNIT;
}

void GLSceneDrawer::drawPitchesGraph(int64_t now) {
    glColor3f(1.0, 0.85, 0.35);

    auto getXAndY = [&](const SingerPitchDetection item, double* x, double* y) {
        *x = (item.time + DISPLAY_PITCH_TIME_LIMIT - now) / DISPLAY_PITCH_TIME_LIMIT - 1.0;
        *y = getPitchPosition(item.pitch);
    };

    auto current = std::find_if(detectedPitches.crbegin(), detectedPitches.crend(),
            [](const SingerPitchDetection& item) {
        return item.pitch.hasPerfectFrequency();
    });

    Pitch currentPitch;
    if (current == detectedPitches.crend()) {
        return;
    } else {
        currentPitch = current->pitch;
    }

    for (auto iter = detectedPitches.begin(); iter != detectedPitches.end() - 1; iter++) {
        if (iter->pitch.getOctave() != currentPitch.getOctave()) {
            continue;
        }

        auto next = iter + 1;
        if (next->pitch.getOctave() != currentPitch.getOctave()) {
            continue;
        }

        double x, y;
        getXAndY(*iter, &x, &y);

        double nextX, nextY;
        getXAndY(*next, &nextX, &nextY);

        glBegin(GL_LINES);
        {
            glVertex2f((GLfloat) x, (GLfloat) y);
            glVertex2f((GLfloat) nextX, (GLfloat) nextY);
        }
        glEnd();
    }
}

void GLSceneDrawer::drawWavPitches(int64_t now) {
    if (pitchesFromWavFile.empty()) {
        return;
    }

    glColor3f(0.3, 0.2, 0.67);
    glBegin(GL_POLYGON);
    int64_t pitchSummaryTime = 0;
    for (PitchDetection pitchDetection : pitchesFromWavFile) {
        int64_t pitchBeginTime = pitchSummaryTime;
        pitchSummaryTime += pitchDetection.durationInMicroSeconds;

        if (!pitchDetection.pitch.isValid()) {
            continue;
        }

        double x1Time = pitchesLoadedTime - now + pitchBeginTime;
        double x2Time = pitchesLoadedTime - now + pitchSummaryTime;
        if (x2Time < 0) {
            continue;
        }

        float x1;
        if (x1Time < 0) {
            x1 = 0.f;
        } else if(x1Time >= DISPLAY_PITCH_TIME_LIMIT) {
            continue;
        } else {
            x1 = (float) (x1Time / DISPLAY_PITCH_TIME_LIMIT);
        }

        float x2 = (float) (x2Time / DISPLAY_PITCH_TIME_LIMIT);
        if (x2 > 1.f) {
            continue;
        }

        float y1;
        float y2;
        getPitchLowerAndUpperBoundsPositions(pitchDetection.pitch, &y1, &y2);

        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    }
    glEnd();
}

void GLSceneDrawer::drawDividers() {
    glColor3f(0.5, 0.85, 0.42);
    glBegin(GL_LINES);
    float pitchUnit = 2.0f / Pitch::PITCHES_IN_OCTAVE;
    float startPosition = -1.0f + pitchUnit;
    for (int i = 0; i < Pitch::PITCHES_IN_OCTAVE - 1; ++i) {
        glVertex2f(-1.0f, startPosition);
        glVertex2f(1.0f, startPosition);
        startPosition += pitchUnit;
    }
    glEnd();
}

GLSceneDrawer::GLSceneDrawer() {
    pitchesLoadedTime = -1;

    studentPitchInputReader = new PitchInputReader(CreateDefaultAudioInputReader(1200));
    studentPitchInputReader->setThreshold(0.25);
    studentPitchInputReader->setExecuteCallBackOnInvalidPitches(true);
    studentPitchInputReader->setCallback([this](Pitch pitch) {
        this->studentPitchDetected(pitch);
    });
    studentPitchInputReader->start();
}

void GLSceneDrawer::readPitchesFromWav(const char *wavFileName) {
    std::fstream file(wavFileName);
    WavFilePitchesReader reader;
    float threshold = 0.15;
    pitchesFromWavFile = reader.readPitchesFromWavFile(&file, threshold);
    pitchesLoadedTime = TimeUtils::nowInMicroseconds();
}

void GLSceneDrawer::studentPitchDetected(const Pitch &pitch) {
    int64_t time = TimeUtils::nowInMicroseconds();
    detectedPitches.push_back(SingerPitchDetection(pitch, time));
}

GLSceneDrawer::~GLSceneDrawer() {
    delete studentPitchInputReader;
}

const std::vector<GLSceneDrawer::SingerPitchDetection> &GLSceneDrawer::getDetectedPitches() const {
    return detectedPitches;
}
