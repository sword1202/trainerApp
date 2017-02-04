#ifndef DRAW_CONSTANTS_PITCHGRAPHDRAWER_H
#define DRAW_CONSTANTS_PITCHGRAPHDRAWER_H

#import <cstdint>
#import "Pitch.h"

static const int64_t DISPLAY_PITCH_TIME_LIMIT = 5000000; // in microseconds
static const float PITCH_UNIT = 2.0f / Pitch::PITCHES_IN_OCTAVE;

#endif