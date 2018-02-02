//
// Created by Semyon Tikhonenko on 1/12/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#include "GLSceneDrawer.h"
#include "TimeUtils.h"
#include "DrawConstants.h"
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#else
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#endif
#include <iostream>

using std::cerr;
using std::endl;

static const int STUDENT_PITCH_SMOOTH_LEVEL = 4;
// minimum pitch detection probability is 0.75. Pitches with lower probability are marked as invalid
static const float PITCH_DETECTION_THRESHOLD = 0.25;
static const int PITCH_DETECTION_BUFFER_SIZE = 1200;

void GLSceneDrawer::draw(int width, int height) {
    glClear(GLenum(GL_COLOR_BUFFER_BIT));
    int64_t now = TimeUtils::NowInMicroseconds();
    glViewport(0, 0, width, height);

    drawBlackPitchesBackground();

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
    //drawDividers();
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

void GLSceneDrawer::drawBlackPitchesBackground() {
    glColor3f(PIANO_BLACK_PITCH_BACKGROUND_COLOR);

    float startPosition = -1.0f;
    for (int i = 0; i < Pitch::PITCHES_IN_OCTAVE; ++i) {
        if (!Pitch::isWhite(i)) {
            glBegin(GL_POLYGON);
            {
                glVertex2f(-1.0f, startPosition);
                glVertex2f(1.0f, startPosition);
                glVertex2f(1.0f, startPosition + PITCH_UNIT);
                glVertex2f(-1.0f, startPosition + PITCH_UNIT);
            }
            glEnd();
        }

        startPosition += PITCH_UNIT;
    }

    float efPitchDividerPosition = -1.0f + PITCH_UNIT * 5;
    glBegin(GL_LINES);
    {
        glVertex2f(-1.0f, efPitchDividerPosition);
        glVertex2f(1.0f, efPitchDividerPosition);
    }
    glEnd();
}

GLSceneDrawer::GLSceneDrawer() {
    pitchesLoadedTime = -1;

    AudioInputReader *audioInputReader = CreateDefaultAudioInputReader(PITCH_DETECTION_BUFFER_SIZE);
    studentPitchInputReader = new PitchInputReader(audioInputReader, STUDENT_PITCH_SMOOTH_LEVEL);
    studentPitchInputReader->setThreshold(PITCH_DETECTION_THRESHOLD);
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
    pitchesLoadedTime = TimeUtils::NowInMicroseconds();
}

void GLSceneDrawer::studentPitchDetected(const Pitch &pitch) {
    std::lock_guard<std::mutex> _(pitchesMutex);
    studentPitchGraphDrawer.onPitchDetected(pitch);
}

GLSceneDrawer::~GLSceneDrawer() {
    delete studentPitchInputReader;
}

void GLSceneDrawer::setMoveBetweenOctaves(bool moveBetweenOctaves) {
    studentPitchGraphDrawer.setMoveBetweenOctaves(moveBetweenOctaves);
}
