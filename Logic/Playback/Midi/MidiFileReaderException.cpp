//
// Created by Semyon Tykhonenko on 2019-05-19.
//

#include "MidiFileReaderException.h"

const char *MidiFileReaderException::what() const noexcept {
    switch (reason) {
        case OUT_OF_RANGE:
            return "No track with specified index available";
        case DRUM_TRACK:
            return "Failed to convert drums track into vocal part";
        case HAS_UNSUPPORTED_PITCHES:
            return "The specified track contains unsupported pitches";
        case OUT_OF_MAX_SUPPORTED_POLYPHONIC_TRACKS_COUNT:
            return "The track reaches out the maximum number of simultaneously playing pitches";
        default:
            return "unknown error";
    }
}

MidiFileReaderException::MidiFileReaderException(MidiFileReaderException::Reason reason) : reason(reason) {}

MidiFileReaderException::Reason MidiFileReaderException::getReason() const {
    return reason;
}
