//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PianoDrawer.h"
#include "Drawer.h"
#include "MainController.h"

constexpr float bigPitchHeight = 25.5;
constexpr float smallPitchHeight = 18.75;
constexpr float sharpPitchHeight = 9.75;
constexpr float sharpPitchWidth = 22.5;
constexpr float distanceBetweenPitches = 2;
constexpr float pianoWidth = 51;

constexpr float pitchRadius = 3;
constexpr float sharpPitchRadius = 1.5;
constexpr float distanceBetweenTextLeftAndPitchRight = 18.0;

constexpr float heightMap[] = {smallPitchHeight, bigPitchHeight,
        smallPitchHeight, smallPitchHeight, bigPitchHeight, bigPitchHeight, smallPitchHeight};
constexpr int heightMapLength = 7;
constexpr bool hasSharpMap[] = {true, true, false, true, true, true, false};
static const Drawer::Color borderColor = {0x9A, 0x98, 0xD0, 0x7f};
static const Drawer::Color sharpPitchColor = {0x9A, 0x98, 0xD0, 0xff};
static const Drawer::Color reachedPitchColor = {0x31, 0xDD, 0x6C, 0xff};
static const Drawer::Color missedPitchColor = {0xFF, 0x5E, 0x85, 0xff};
static const Drawer::Color selectedPitchColor = {0x61, 0x5F, 0x97, 0xff};
static const Drawer::Color pitchTextColor = {0x24, 0x23, 0x2D, 0xe6};
static const Drawer::Color selectedPitchTextColor = {0xff, 0xff, 0xff, 0xff};

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

PianoDrawer::PianoDrawer(Drawer *drawer)
        : drawer(drawer){
    drawSharpPitchesFillColor.reserve(100);
    drawSharpPitchesY.reserve(100);
    intervalHeight = 0;
    firstPitchIndex = -1;
    detectedPitchIndex = -1;
}

void PianoDrawer::draw(float width, float height, float devicePixelRation) {
    assert(intervalHeight > 0);

    drawer->clear();

    drawer->setStrokeColor(borderColor);
    drawer->setFillColor(sharpPitchColor);

    int index = getFirstPitch().getWhiteIndex();
    int perfectFrequencyIndex = firstPitchIndex;
    float y = height;

    drawSharpPitchesY.clear();
    drawSharpPitchesFillColor.clear();
    selectedWhitePitchIndexes.clear();

    float intervalOctaveHeightToPianoOctaveHeightRelation = getIntervalOctaveHeightToPianoOctaveHeightRelation();

    while (y > -bigPitchHeight) {

        bool fill = false;

        auto getFillColor = [&] (int perfectFrequencyIndex) {
            Pitch detectedPitch = Pitch::fromPerfectFrequencyIndex(detectedPitchIndex);
            if (detectedPitchIndex == perfectFrequencyIndex) {
                if (pitchSequence->hasPitchNow(detectedPitch)) {
                    return reachedPitchColor;
                } else {
                    return selectedPitchColor;
                }
            } else {
                if (pitchSequence->hasPitchNow(Pitch::fromPerfectFrequencyIndex(perfectFrequencyIndex)) &&
                        !pitchSequence->hasPitchNow(detectedPitch)) {
                    return missedPitchColor;
                } else {
                    return sharpPitchColor;
                }
            }
        };

        Drawer::Color fillColor = getFillColor(perfectFrequencyIndex);
        if (fillColor != sharpPitchColor) {
            fill = true;
            drawer->setFillColor(fillColor);
            selectedWhitePitchIndexes.insert(index);
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

    drawer->setTextAlign(Drawer::LEFT);
    drawer->setTextBaseline(Drawer::MIDDLE);
    drawer->setTextFont("Lato-Bold", 10);

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

        if (selectedWhitePitchIndexes.count(index)) {
            drawer->setFillColor(selectedPitchTextColor);
        } else {
            drawer->setFillColor(pitchTextColor);
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
