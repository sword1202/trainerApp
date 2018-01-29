//
// Created by Semyon Tikhonenko on 2/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#include "PianoDrawer.h"
#include "DrawConstants.h"
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#else
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#endif

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

void PianoDrawer::draw(const Pitch& pitch) {
    glColor3f(PIANO_WHITE_BUTTON_COLOR);
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

    setupPitchSelectorDrawing(pitch);

    if (selectorDrawType == WHITE) {
        drawPitchSelector();
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
    glColor3f(PIANO_BLACK_BUTTON_COLOR);
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

    if (selectorDrawType == BLACK) {
        drawPitchSelector();
    }
}

void PianoDrawer::drawPitchSelector() {
    glColor3f(PIANO_SELECTION_COLOR);
    glBegin(GL_POLYGON);
    {
        glVertex2f(selectedPitchX1, selectedPitchY1);
        glVertex2f(selectedPitchX2, selectedPitchY1);
        glVertex2f(selectedPitchX2, selectedPitchY2);
        glVertex2f(selectedPitchX1, selectedPitchY2);
    }
    glEnd();
}

void PianoDrawer::setupPitchSelectorDrawing(const Pitch& pitch) {
    if (!pitch.isValid()) {
        selectorDrawType = NONE;
        return;
    }

    int pitchInOctaveIndex = pitch.getPitchInOctaveIndex();
    if (pitchInOctaveIndex >= 0) {
        if (Pitch::isWhite(pitchInOctaveIndex)) {
            selectedPitchX2 = -1.0f + PIANO_WIDTH;
            int whitePitchIndex = Pitch::getWhitePitchInOctaveIndex(pitchInOctaveIndex);
            selectedPitchY1 = whitePitchIndex * DIVIDER_OFFSET - 1.0f;
            selectedPitchY2 = selectedPitchY1 + DIVIDER_OFFSET;
            selectorDrawType = WHITE;
        } else {
            selectedPitchX2 = -1.0f + PIANO_WIDTH * BLACK_WHITE_PIANO_BUTTON_RELATION;
            int blackPointIndex = Pitch::getBlackPitchInOctaveIndex(pitchInOctaveIndex);

            const float* blackPoint = BLACK_POINTS[blackPointIndex];
            selectedPitchY1 = blackPoint[0] - 1.0f;
            selectedPitchY2 = blackPoint[1] - 1.0f;
            selectorDrawType = BLACK;
        }
    }
}
