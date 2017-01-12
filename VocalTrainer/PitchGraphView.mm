//
// Created by Semyon Tikhonenko on 8/10/16.
// Copyright (c) 2016 Mac. All rights reserved.
//

#import "PitchGraphView.h"
#include <GLKit/GLKit.h>
#include <GLUT/GLUT.h>
#include "PitchInputReader.h"
#import "WavFileAudioInputReader.h"
#import "WavFilePitchesReader.h"
#import <iostream>

static const double DISPLAY_PITCH_TIME_LIMIT = 5.0;
static const float PITCH_UNIT = 2.0f / Pitch::PITCHES_IN_OCTAVE;

struct SingerPitchDetection {
    double time;
    Pitch pitch;

    SingerPitchDetection(const Pitch &pitch, double time) : pitch(pitch), time(time) {
    }
};

@implementation PitchGraphView {
    PitchInputReader* _studentPitchInputReader;
    std::vector<SingerPitchDetection> detectedPitches;
    std::vector<PitchDetection> pitchesFromWavFile;
    double pitchesLoadedTime;
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
        [self setupStudentPitchInputReader];
        pitchesLoadedTime = -1;
    }

    return self;
}

- (void)readPitchesFromWav:(NSString*)wavFile {
    std::fstream file(wavFile.UTF8String);
    WavFilePitchesReader reader;
    float threshold = 0.15;
    pitchesFromWavFile = reader.readPitchesFromWavFile(&file, threshold);
    pitchesLoadedTime = [[NSDate date] timeIntervalSince1970];
}

- (void)setupStudentPitchInputReader {
    _studentPitchInputReader = new PitchInputReader(CreateDefaultAudioInputReader(1200));
    _studentPitchInputReader->setThreshold(0.05);
    __weak PitchGraphView * weakSelf = self;
    _studentPitchInputReader->setCallback([weakSelf](Pitch pitch) {
            if (PitchGraphView * strongSelf = weakSelf) {
                [strongSelf studentPitchDetected:pitch];
            }
        });
    _studentPitchInputReader->start();
}

- (void)studentPitchDetected:(Pitch)pitch {
    if (!pitch.hasPerfectFrequency()) {
        return;
    }

    double time = [[NSDate date] timeIntervalSince1970];
    detectedPitches.push_back(SingerPitchDetection(pitch, time));
}

- (void)onTimer:(id)onTimer {
    [self display];
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

- (void)drawRect:(NSRect)dirtyRect {
    double now = [[NSDate date] timeIntervalSince1970];
    auto iter = std::find_if(detectedPitches.begin(), detectedPitches.end(),
            [=] (const SingerPitchDetection& pitchDetection) {
        return now - pitchDetection.time <= DISPLAY_PITCH_TIME_LIMIT;
    });

    detectedPitches.erase(detectedPitches.begin(), iter);

    glClear(GLenum(GL_COLOR_BUFFER_BIT));

    [self drawPitchesGraph:now];
    [self drawWavPitches:now];

    [self drawDividers];

    glFlush();
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
        NSLog(@"Gl error: %d", glError);
    }
}

- (void)drawWavPitches:(double)now {
    if (pitchesFromWavFile.empty()) {
        return;
    }

    glColor3f(0.3, 0.2, 0.67);
    glBegin(GL_POLYGON);
    int64_t pitchSummaryTime = 0;
    for (PitchDetection pitchDetection : pitchesFromWavFile) {
        int64_t pitchBeginTime = pitchSummaryTime;
        pitchSummaryTime += pitchDetection.timeInMicroSeconds;

        if (!pitchDetection.pitch.isValid()) {
            continue;
        }

        double x1Time = pitchesLoadedTime - now + (double)pitchBeginTime / 1000000.0;
        double x2Time = pitchesLoadedTime - now + (double)pitchSummaryTime / 1000000.0;
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

- (void)drawPitchesGraph:(double)now {
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

- (void)drawDividers {
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

- (void)onWavFileSelected:(NSString *)path {
    [self readPitchesFromWav:path];
}

- (void)dealloc {
    delete _studentPitchInputReader;
}


@end