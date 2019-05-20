//
// Created by Semyon Tykhonenko on 2019-05-19.
//

#ifndef TEXTIMAGESGENERATOR_MIDIFILEREADEREXCEPTION_H
#define TEXTIMAGESGENERATOR_MIDIFILEREADEREXCEPTION_H

#include <exception>

class MidiFileReaderException : public std::exception {
public:
    enum Reason {
        OUT_OF_RANGE,
        DRUM_TRACK,
        HAS_UNSUPPORTED_PITCHES,
        OUT_OF_MAX_SUPPORTED_POLYPHONIC_TRACKS_COUNT
    };
private:
    Reason reason;
public:

    explicit MidiFileReaderException(Reason reason);
    const char *what() const noexcept override;
    Reason getReason() const;
};


#endif //TEXTIMAGESGENERATOR_MIDIFILEREADEREXCEPTION_H
