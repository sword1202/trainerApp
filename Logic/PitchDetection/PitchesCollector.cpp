
#include "PitchesCollector.h"

Pitch PitchesCollector::getPitchAt(int index) const {
    return Pitch(getFrequencyAt(index));
}
