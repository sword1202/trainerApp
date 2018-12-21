
#include "PitchesRecorder.h"

Pitch PitchesRecorder::getPitchAt(int index) const {
    return Pitch(getFrequencyAt(index));
}
