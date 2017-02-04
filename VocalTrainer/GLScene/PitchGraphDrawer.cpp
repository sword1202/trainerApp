//
// Created by Semyon Tikhonenko on 2/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#include "PitchGraphDrawer.h"
#include "TimeUtils.h"
#include "DrawConstants.h"
#include <GLUT/GLUT.h>

using namespace Azazai;

void PitchGraphDrawer::onPitchDetected(const Pitch &pitch) {
    int64_t time = TimeUtils::nowInMicroseconds();
    detectedPitches.push_back(SingerPitchDetection(pitch, time));
}

static double getPitchPosition(const Pitch& pitch) {
    int pitchIndexInOctave = pitch.getPitchInOctaveIndex();
    float lowerBoundPosition = PITCH_UNIT * pitchIndexInOctave;
    return (lowerBoundPosition + pitch.getDistanceFromLowerBound() / 2 * PITCH_UNIT) - 1.0f;
}

void PitchGraphDrawer::draw(int64_t now) {
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

const std::vector<SingerPitchDetection> & PitchGraphDrawer::updatePitches(int64_t now) {
    auto iter = std::find_if(detectedPitches.begin(), detectedPitches.end(),
            [=](const SingerPitchDetection &pitchDetection) {
                return now - pitchDetection.time <= DISPLAY_PITCH_TIME_LIMIT;
            });

    detectedPitches.erase(detectedPitches.begin(), iter);
    return detectedPitches;
}
