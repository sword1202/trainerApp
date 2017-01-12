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

void GLSceneDrawer::draw() {
    int64_t now = TimeUtils::nowInMicroseconds();
    auto iter = std::find_if(detectedPitches.begin(), detectedPitches.end(),
            [=](const SingerPitchDetection &pitchDetection) {
                return now - pitchDetection.time <= DISPLAY_PITCH_TIME_LIMIT;
            });

    detectedPitches.erase(detectedPitches.begin(), iter);

    glClear(GLenum(GL_COLOR_BUFFER_BIT));
    drawPitchesGraph(now);
    drawWavPitches(now);
    drawDividers();

    glFlush();
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
        cerr<<"Gl error: "<<glError<<endl;
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
    glBegin(GL_LINES);
    int index = 0;
    for (const auto& item : detectedPitches) {
        double x = (item.time + DISPLAY_PITCH_TIME_LIMIT - now) / DISPLAY_PITCH_TIME_LIMIT - 1.0;
        double y = getPitchPosition(item.pitch);

        if (index > 0 && index < detectedPitches.size() - 1) {
            glVertex2f((GLfloat) x, (GLfloat) y);
        }
        glVertex2f((GLfloat) x, (GLfloat) y);

        index++;
    }
    glEnd();
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
    studentPitchInputReader->setThreshold(0.05);
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
    if (!pitch.hasPerfectFrequency()) {
        return;
    }

    int64_t time = TimeUtils::nowInMicroseconds();
    detectedPitches.push_back(SingerPitchDetection(pitch, time));
}

GLSceneDrawer::~GLSceneDrawer() {
    delete studentPitchInputReader;
}
