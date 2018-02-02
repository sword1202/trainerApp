//
// Created by Semyon Tikhonenko on 2/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#include "PitchGraphDrawer.h"
#include "TimeUtils.h"
#include "DrawConstants.h"
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#else
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <algorithm>
#endif
#include <iostream>
#include "GeometryUtils.h"

using namespace CppUtils;
using std::cout;
using std::endl;

void PitchGraphDrawer::onPitchDetected(const Pitch &pitch) {
    int64_t time = TimeUtils::NowInMicroseconds();
    detectedPitches.push_back(SingerPitchDetection(pitch, time));
}

static GLfloat getPitchPosition(const Pitch& pitch) {
    int pitchIndexInOctave = pitch.getPitchInOctaveIndex();
    float lowerBoundPosition = PITCH_UNIT * pitchIndexInOctave;
    return (lowerBoundPosition + pitch.getDistanceFromLowerBound() / 2 * PITCH_UNIT) - 1.0f;
}

static void getXAndY(int64_t now, SingerPitchDetection& item, GLfloat *x, GLfloat *y) {
    *x = (GLfloat) ((item.time + DISPLAY_PITCH_TIME_LIMIT - now) / DISPLAY_PITCH_TIME_LIMIT - 1.0);
    *y = getPitchPosition(item.pitch);
}

void PitchGraphDrawer::draw(int64_t now) {
    glColor3f(PITCH_GRAPH_COLOR);

    Pitch currentPitch;
    if (moveBetweenOctaves) {
        auto current = std::find_if(detectedPitches.crbegin(), detectedPitches.crend(),
                [](const SingerPitchDetection& item) {
                    return item.pitch.hasPerfectFrequency();
                });

        if (current == detectedPitches.crend()) {
            return;
        } else {
            currentPitch = current->pitch;
        }
    }

    auto checkPitch = [&](const Pitch& pitch) {
        if (moveBetweenOctaves) {
            if (pitch.getOctave() != currentPitch.getOctave()) {
                return false;
            }
        } else if(!pitch.hasPerfectFrequency()) {
            return false;
        }

        return true;
    };

    for (auto iter = detectedPitches.begin(); iter != detectedPitches.end() - 1; iter++) {
        if (!checkPitch(iter->pitch)) {
            continue;
        }

        auto next = iter + 1;
        if (next == detectedPitches.end()) {
            break;
        }

        if (!checkPitch(next->pitch)) {
            continue;
        }

        GLfloat x, y;
        getXAndY(now, *iter, &x, &y);

        GLfloat nextX, nextY;
        getXAndY(now, *next, &nextX, &nextY);

        if (moveBetweenOctaves || iter->pitch.getOctave() == next->pitch.getOctave()) {
            glBegin(GL_LINES);
            {
                glVertex2f(x, y);
                glVertex2f(nextX, nextY);
            }
            glEnd();
        } else {
            GLfloat intersectionX;
            GLfloat currentPitchIntersectionY,
                    nextPitchIntersectionY;
            if (nextY > y) {
                GeometryUtils::GetLinesIntersection(
                        // line1
                        x, y,
                        nextX, nextY,
                        //line2
                        0.0f, -1.0f,
                        1.0f, -1.0f,
                        // result
                        &intersectionX, (GLfloat*)0);

                currentPitchIntersectionY = -1.0f;
                nextPitchIntersectionY = 1.0f;
            } else {
                GeometryUtils::GetLinesIntersection(
                        // line1
                        x, y,
                        nextX, nextY,
                        //line2
                        0.0f, 1.0f,
                        1.0f, 1.0f,
                        // result
                        &intersectionX, (GLfloat*)0);

                currentPitchIntersectionY = 1.0f;
                nextPitchIntersectionY = -1.0f;
            }

            glBegin(GL_LINES);
            {
                glVertex2f(x, y);
                glVertex2f(intersectionX, currentPitchIntersectionY);
            }
            glEnd();

            glBegin(GL_LINES);
            {
                glVertex2f(intersectionX, nextPitchIntersectionY);
                glVertex2f(nextX, nextY);
            }
            glEnd();
        }
    }
}

const std::vector<SingerPitchDetection> & PitchGraphDrawer::updatePitches(int64_t now) {
    auto iter = std::find_if(detectedPitches.begin(), detectedPitches.end(),
            [=](const SingerPitchDetection &pitchDetection) {
                return now - pitchDetection.time <= DISPLAY_PITCH_TIME_LIMIT;
            });

    detectedPitches.erase(detectedPitches.begin(), iter);
    return detectedPitches;
}

void PitchGraphDrawer::setMoveBetweenOctaves(bool moveBetweenOctaves) {
    this->moveBetweenOctaves = moveBetweenOctaves;
}
