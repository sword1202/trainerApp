//
// Created by Semyon Tikhonenko on 8/10/16.
// Copyright (c) 2016 Mac. All rights reserved.
//

#import "PitchGraphView.h"
#include <GLKit/GLKit.h>
#include <GLUT/GLUT.h>
#include "PitchInputReader.h"
#include <vector>

static const double DISPLAY_PITCH_TIME_LIMIT = 5.0;

struct PitchDetection {
    double time;
    Pitch pitch;

    PitchDetection(const Pitch &pitch, double time) : pitch(pitch), time(time) {
    }
};

@implementation PitchGraphView {
    PitchInputReader* _pitchInputReader;
    std::vector<PitchDetection> detectedPitches;
}

- (void)prepareOpenGL {
    [_window makeFirstResponder:self];
    glClearColor(0, 0, 0, 0);

    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
                                     target:self
                                   selector:@selector(onTimer:)
                                   userInfo:nil
                                    repeats:YES];
}

- (id)initWithCoder:(NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        _pitchInputReader = new PitchInputReader(CreateAudioInputReader(2400));
        _pitchInputReader->setThreshold(0.05);
        __weak PitchGraphView * weakSelf = self;
        _pitchInputReader->setCallback([weakSelf](Pitch pitch) {
            if (PitchGraphView * strongSelf = weakSelf) {
                [strongSelf pitchDetected:pitch];
            }
        });
        _pitchInputReader->start();
    }

    return self;
}

- (void)pitchDetected:(Pitch)pitch {
    double time = [[NSDate date] timeIntervalSince1970];
    detectedPitches.push_back(PitchDetection(pitch, time));
}

- (void)onTimer:(id)onTimer {
    [self display];
}

static double getPitchPosition(const Pitch& pitch) {
    int pitchIndexInOctave = pitch.getPitchInOctaveIndex();
    float pitchUnit = 2.0f / Pitch::PITCHES_IN_OCTAVE;
    float lowerBoundPosition = pitchUnit * pitchIndexInOctave;
    return (lowerBoundPosition + pitch.getDistanceFromLowerBound() / 2 * pitchUnit) - 1.0f;
}

- (void)drawRect:(NSRect)dirtyRect {
    double now = [[NSDate date] timeIntervalSince1970];
    auto iter = std::find_if(detectedPitches.begin(), detectedPitches.end(), [=](const PitchDetection& pitchDetection) {
        return now - pitchDetection.time <= DISPLAY_PITCH_TIME_LIMIT;
    });

    detectedPitches.erase(detectedPitches.begin(), iter);

    glClear(GLenum(GL_COLOR_BUFFER_BIT));

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
    glFlush();
    assert(glGetError() == GL_NO_ERROR);
}

- (void)dealloc {
    delete _pitchInputReader;
}


@end