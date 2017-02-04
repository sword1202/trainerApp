//
// Created by Semyon Tikhonenko on 1/12/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#include "GLSceneDrawer.h"
#include "TimeUtils.h"
#include "DrawConstants.h"
#include <GLUT/GLUT.h>
#include <iostream>

using std::cerr;
using std::endl;

static const int STUDENT_PITCH_SMOOTH_LEVEL = 4;

void GLSceneDrawer::draw(int width, int height) {
    glClear(GLenum(GL_COLOR_BUFFER_BIT));
    int64_t now = TimeUtils::nowInMicroseconds();
    glViewport(0, 0, width, height);

    Pitch currentPitch;
    {
        std::lock_guard<std::mutex> _(pitchesMutex);
        const auto& detectedPitches = studentPitchGraphDrawer.updatePitches(now);
        if (!detectedPitches.empty()) {
            currentPitch = detectedPitches.back().pitch;
        }

        studentPitchGraphDrawer.draw(now);
    }
    drawWavPitches(now);
    drawDividers();
    pianoDrawer.draw(currentPitch);

    glFlush();
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
        cerr<<"Gl error: "<<glError<<endl;
    }
}

static void getPitchLowerAndUpperBoundsPositions(const Pitch& pitch, float *outLower, float *outUpper) {
    int pitchIndexInOctave = pitch.getPitchInOctaveIndex();
    *outLower = PITCH_UNIT * pitchIndexInOctave - 1.0f;
    *outUpper = *outLower + PITCH_UNIT;
}

void GLSceneDrawer::drawWavPitches(int64_t now) {
    if (pitchesFromWavFile.empty()) {
        return;
    }

    glColor3f(0.3, 0.2, 0.67);
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

        glBegin(GL_POLYGON);
        {
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
        }
        glEnd();
    }
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

    studentPitchInputReader = new PitchInputReader(CreateDefaultAudioInputReader(1200), STUDENT_PITCH_SMOOTH_LEVEL);
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
    float threshold = 0.05;
    pitchesFromWavFile = reader.readPitchesFromWavFile(&file, threshold);
    pitchesLoadedTime = TimeUtils::nowInMicroseconds();
}

void GLSceneDrawer::studentPitchDetected(const Pitch &pitch) {
    std::lock_guard<std::mutex> _(pitchesMutex);
    studentPitchGraphDrawer.onPitchDetected(pitch);
}

GLSceneDrawer::~GLSceneDrawer() {
    delete studentPitchInputReader;
}

bool GLSceneDrawer::getMoveBetweenOctaves() const {
    return moveBetweenOctaves;
}

void GLSceneDrawer::setMoveBetweenOctaves(bool moveBetweenOctaves) {
    this->moveBetweenOctaves = moveBetweenOctaves;
}
