﻿//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PianoDrawer.h"
#include "Drawer.h"

constexpr float bigPitchHeight = 25.5;
constexpr float smallPitchHeight = 18.75;
constexpr float sharpPitchHeight = 9.75;
constexpr float sharpPitchWidth = 22.5;
constexpr float distanceBetweenPitches = 2;
constexpr float pianoWidth = 51;

constexpr float pitchRadius = 3;
constexpr float sharpPitchRadius = 2;
constexpr float distanceBetweenTextLeftAndPitchRight = 18.0;

constexpr float heightMap[] = {smallPitchHeight, bigPitchHeight,
        smallPitchHeight, smallPitchHeight, bigPitchHeight, bigPitchHeight, smallPitchHeight};
constexpr int heightMapLength = 7;
constexpr bool hasSharpMap[] = {true, true, false, true, true, true, false};

typedef unsigned char uchar;
const Drawer::Color PianoDrawer::PITCH_TEXT_COLOR(uchar(0x24), uchar(0x23), uchar(0x2D), uchar(0xe6));
const Drawer::Color PianoDrawer::SELECTED_PITCH_TEXT_COLOR(Drawer::Color::white());

constexpr int FONT_WEIGHT = 60;

float PianoDrawer::getIntervalOctaveHeightToPianoOctaveHeightRelation() const {
    // 12 pitches in octave
    float intervalOctaveHeight = intervalHeight * 12;
    // 4 small pitches and 3 big pitches from heightMap, 5: 7 pitches in octave
    float pianoOctaveHeight = smallPitchHeight * 4 + bigPitchHeight * 3 + distanceBetweenPitches * 7;
    return intervalOctaveHeight / pianoOctaveHeight;
}

void PianoDrawer::setIntervalHeight(float intervalHeight) {
    this->intervalHeight = intervalHeight;
}

PianoDrawer::PianoDrawer(Drawer *drawer, const WorkspaceColorScheme* colors)
        : drawer(drawer), colors(colors) {
    drawSharpPitchesFillColor.reserve(100);
    drawSharpPitchesY.reserve(100);
    intervalHeight = 0;
    firstPitchIndex = -1;
    detectedPitchIndex = -1;
}

void PianoDrawer::draw(float width, float height, float devicePixelRation) {
    assert(intervalHeight > 0);
    assert(pitchSequence != nullptr);

    drawer->setTextAlign(Drawer::LEFT);
    drawer->setTextBaseline(Drawer::MIDDLE);
    drawer->setTextFontSize(fontSize);
    drawer->setTextStyle(fontStyle);

    drawer->setStrokeColor(colors->pianoBorderColor);
    drawer->setFillColor(colors->pianoSharpPitchColor);

    int index = getFirstPitch().getWhiteIndex();
    int perfectFrequencyIndex = firstPitchIndex;
    float y = height;

    drawSharpPitchesY.clear();
    drawSharpPitchesFillColor.clear();
    selectedPitchIndexes.clear();

    float intervalOctaveHeightToPianoOctaveHeightRelation = getIntervalOctaveHeightToPianoOctaveHeightRelation();

    while (y > -bigPitchHeight) {

        bool fill = false;

        auto getFillColor = [&] (int perfectFrequencyIndex) {
            Pitch detectedPitch = Pitch::fromPerfectFrequencyIndex(detectedPitchIndex);
            if (detectedPitchIndex == perfectFrequencyIndex) {
                if (pitchSequence->hasPitchNow(detectedPitch)) {
                    return colors->reachedPitchColor;
                } else {
                    return colors->pianoSelectedPitchColor;
                }
            } else {
                if (pitchSequence->hasPitchNow(Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex)) &&
                        !pitchSequence->hasPitchNow(detectedPitch)) {
                    return colors->missedPitchColor;
                } else {
                    return colors->pianoSharpPitchColor;
                }
            }
        };

        Drawer::Color fillColor = getFillColor(perfectFrequencyIndex);
        if (fillColor != colors->pianoSharpPitchColor) {
            fill = true;
            drawer->setFillColor(fillColor);
            selectedPitchIndexes.insert(perfectFrequencyIndex);
        }

        float pitchHeight = heightMap[index % heightMapLength] * intervalOctaveHeightToPianoOctaveHeightRelation;
        drawer->roundedRectDifferentCorners(0, y - pitchHeight, pianoWidth - 1, pitchHeight, 0,
                pitchRadius * intervalOctaveHeightToPianoOctaveHeightRelation,
                pitchRadius * intervalOctaveHeightToPianoOctaveHeightRelation, 0);
        if (fill) {
            drawer->fill();
        } else {
            drawer->stroke();
        }

        y -= pitchHeight;

        // post draw sharp pitch command
        if (hasSharpMap[index % heightMapLength]) {
            perfectFrequencyIndex++;
            fillColor = getFillColor(perfectFrequencyIndex);
            drawSharpPitchesFillColor.push_back(fillColor);
            drawSharpPitchesY.push_back(y);
        }

        y -= distanceBetweenPitches * intervalOctaveHeightToPianoOctaveHeightRelation;
        index++;
        perfectFrequencyIndex++;
    }

    drawSharpPitches();
    drawPitchNames(height);
}

void PianoDrawer::drawSharpPitches() const {
    float intervalOctaveHeightToPianoOctaveHeightRelation = getIntervalOctaveHeightToPianoOctaveHeightRelation();

    for (int i = 0; i < drawSharpPitchesY.size(); i++) {
        float y = drawSharpPitchesY[i];
        drawer->setFillColor(drawSharpPitchesFillColor[i]);
        float pitchHeight = sharpPitchHeight * intervalOctaveHeightToPianoOctaveHeightRelation;
        float radius = sharpPitchRadius * intervalOctaveHeightToPianoOctaveHeightRelation;

        drawer->roundedRectDifferentCorners(0,
                y - pitchHeight / 2 - distanceBetweenPitches / 2 * intervalOctaveHeightToPianoOctaveHeightRelation,
                sharpPitchWidth,
                pitchHeight, 0, radius, radius, 0);
        drawer->fill();
    }
}

void PianoDrawer::drawPitchNames(float height) const {
    float y = height;
    Pitch pitch = getFirstPitch();

    float intervalOctaveHeightToPianoOctaveHeightRelation = getIntervalOctaveHeightToPianoOctaveHeightRelation();

    while (y > -bigPitchHeight) {
        int index = pitch.getWhiteIndex();
        int indexInMap = index % heightMapLength;
        float pitchHeight = heightMap[indexInMap] * intervalOctaveHeightToPianoOctaveHeightRelation;

        float textY = y - pitchHeight / 2;
        std::string text;

        if (pitch.getPerfectFrequencyIndex() == firstPitchIndex) {
            text = pitch.getFullName();
        } else if(pitch.getPitchInOctaveIndex() == Pitch::C_INDEX) {
            text = pitch.getFullName();
        } else {
            text = pitch.getName();
        }

        if (selectedPitchIndexes.count(pitch.getPerfectFrequencyIndex())) {
            drawer->setFillColor(SELECTED_PITCH_TEXT_COLOR);
        } else {
            drawer->setFillColor(PITCH_TEXT_COLOR);
        }

        float textX = pianoWidth - distanceBetweenTextLeftAndPitchRight;
        drawer->fillText(text, textX, textY);

        y -= pitchHeight + distanceBetweenPitches * intervalOctaveHeightToPianoOctaveHeightRelation;
        pitch = pitch.getNextWhitePitch();
        if (!pitch.isValid()) {
            break;
        }
    }
}

void PianoDrawer::setFirstPitch(const Pitch &firstPitch) {
    this->firstPitchIndex = firstPitch.getPerfectFrequencyIndex();
}

void PianoDrawer::setDetectedPitch(const Pitch &detectedPitch) {
    this->detectedPitchIndex = detectedPitch.getPerfectFrequencyIndex();
}

void PianoDrawer::setPitchSequence(PlayingPitchSequence *pitchSequence) {
    this->pitchSequence = pitchSequence;
}

Pitch PianoDrawer::getFirstPitch() const {
    return Pitch::fromPerfectFrequencyIndex(firstPitchIndex);
}

void PianoDrawer::setFontSize(int fontSize) {
    this->fontSize = fontSize;
}

void PianoDrawer::setFontStyle(Drawer::FontStyle fontStyle) {
    this->fontStyle = fontStyle;
}

int PianoDrawer::getFontSize() const {
    return fontSize;
}
