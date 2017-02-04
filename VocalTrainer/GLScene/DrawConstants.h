#ifndef DRAW_CONSTANTS_PITCHGRAPHDRAWER_H
#define DRAW_CONSTANTS_PITCHGRAPHDRAWER_H

#import <cstdint>
#import "Pitch.h"

static const int64_t DISPLAY_PITCH_TIME_LIMIT = 5000000; // in microseconds
static const float PITCH_UNIT = 2.0f / Pitch::PITCHES_IN_OCTAVE;

#define PITCH_GRAPH_COLOR 1.0, 0.85, 0.35
#define PIANO_BLACK_BUTTON_COLOR 0.1, 0.1, 0.1
#define PIANO_WHITE_BUTTON_COLOR 0.9, 0.9, 0.9
#define PIANO_SELECTION_COLOR 161 / 255.0f, 204 / 255.0f , 157 / 255.0f

#endif