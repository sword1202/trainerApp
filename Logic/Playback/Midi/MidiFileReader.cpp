//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MidiFileReader.h"
#include "MidiFile.h"
#include "VocalPart.h"
#include "MidiFileReaderException.h"

#include <iostream>
#include <algorithm>

using namespace CppUtils;

static constexpr int    MAX_MIDI_CHANNELS = 15;

static constexpr int    NO_CHANNEL_PREFIX = -1;

static constexpr int    MICROSECONDS_IN_MINUTE = 60'000'000;
static constexpr int    SECONDS_IN_MINUTE = 60;

// MIDI Message = array[status_midi_message_byte, midi_message_byte_1, midi_message_byte_2, midi_message_byte_3, ...]
static constexpr int    STATUS_MIDI_MESSAGE_BYTE_INDEX = 0;
static constexpr int    MIDI_MESSAGE_BYTE_INDEX_1 = 1;
static constexpr int    MIDI_MESSAGE_BYTE_INDEX_2 = 2;
static constexpr int    MIDI_MESSAGE_BYTE_INDEX_3 = 3;
static constexpr int    MIDI_MESSAGE_BYTE_INDEX_4 = 4;
static constexpr int    MIDI_MESSAGE_BYTE_INDEX_5 = 5;

static constexpr int    MIN_NOTE_COUNT = 40;
static constexpr int    MAX_NOTE_COUNT = 1000;
static constexpr int    DRUMS_CHANNEL_ID = 9;
static constexpr int    MAX_SUPPORTED_POLYPHONIC_TRACKS_COUNT = 2;

static constexpr double NAME_WEIGHT = 3.0;
static constexpr double NOTE_COUNT_WEIGHT = 2.0;

static constexpr double LOWEST_NOTE_WEIGHT = 1.0;
static constexpr double LOWEST_NOTE_MX = 58.5;
static constexpr double LOWEST_NOTE_SIGMA = 9.25;

static constexpr double HEIGHEST_NOTE_WEIGHT = 1.0;
static constexpr double HEIGHEST_NOTE_MX = 74.91;
static constexpr double HEIGHEST_NOTE_SIGMA = 8.84;

static constexpr double RANGE_OF_NOTES_WEIGHT = 1.0;
static constexpr double RANGE_OF_NOTES_MX = 16.0;
static constexpr double RANGE_OF_NOTES_SIGMA = 7.0;

static constexpr double MAX_NOTE_VALUE_WEIGHT = 1.0;
static constexpr double MAX_NOTE_VALUE_MX = 63.0;
static constexpr double MAX_NOTE_VALUE_SIGMA = 21.0;

static constexpr double NOTES_PER_SECOND_WEIGHT = 1.0;
static constexpr double NOTES_PER_SECOND_MX = 2.43;
static constexpr double NOTES_PER_SECOND_SIGMA = 1.95;

static constexpr char   SUBSTRING_VOCAL[]  = "vocal";
static constexpr char   SUBSTRING_LEAD[]   = "lead";
static constexpr char   SUBSTRING_SONG[]   = "song";
static constexpr char   SUBSTRING_MELODY[] = "melod";

MidiFileReader::MidiFileReader()
    : durationInSeconds(0.0),
      durationInTicks(0),
      ticksPerQuarter(0),
      ticksPerSecond(0.0),
      beatsPerMinute(0.0),
      ticksPerBeat(0),
      beatsPerTick(0.0),
      currentChannelPrefix(-1)
{

}

MidiFileReader::~MidiFileReader()
{

}

bool MidiFileReader::read(const std::string &filename)
{
    reset();
    MidiFile midifile;
    if (midifile.read(filename) != 0) {
        processMidiFile(midifile);
        return true;
    }

    return false;
}

bool MidiFileReader::read(std::istream &is)
{
    reset();
    MidiFile midifile;
    if (midifile.read(is) != 0) {
        processMidiFile(midifile);
        return true;
    }

    return false;
}

/*!
 * \brief MidiFileReader::reset
 *
 * Clears all data
 */
void MidiFileReader::reset()
{
    durationInSeconds = 0.0;
    durationInTicks = 0;
    ticksPerQuarter = 0;
    ticksPerSecond = 0.0;
    beatsPerMinute = -1;
    ticksPerBeat = 0;
    beatsPerTick = 0.0;
    currentChannelPrefix = NO_CHANNEL_PREFIX;
    tracksMap.clear();
    trackNamesMap.clear();
}

/*!
 * \brief MidiFileReader::processMidiFile
 *
 * Processes MidiFile object
 * \param midi
 * \param outResult
 * \param outBeatsPerMinute
 */
void MidiFileReader::processMidiFile(MidiFile &midi)
{
    currentChannelPrefix = NO_CHANNEL_PREFIX;
    durationInSeconds = midi.getTotalTimeInSeconds();
    durationInTicks = midi.getTotalTimeInTicks();
    ticksPerQuarter = midi.getTPQ();
    ticksPerSecond = durationInTicks / durationInSeconds;

    int trackCount = midi.getTrackCount();
    for (int track = 0; track < trackCount; ++ track) {
        for (size_t event = 0; event < midi[track].size(); ++ event) {
            processEvent(midi[track][event]);
        }
    }
    postProcess();
}

/*!
 * \brief MidiFileReader::processEvent
 *
 * Processes single MIDI or META event
 * \param event
 */
void MidiFileReader::processEvent(const MidiEvent &event)
{
    int eventChannelID = event.getChannel();
    int eventTrackID = event.track;
    int eventTick = event.tick;

    //! ***********************************************
    //! All the data below was got from tutorial      *
    //! http://www.somascape.org/midi/tech/mfile.html *
    //! ***********************************************

    // META events
    if (event.isMeta()) {
        switch (event.getP1()) {
        // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // FF 03 length text.
        // This event is optional. It's interpretation depends on its context. If it occurs in the first track of a format 0 or 1 MIDI file, then it gives the Sequence Name. Otherwise it gives the Track Name
        case 0x03: {
            std::string trackName = eventText(event, MIDI_MESSAGE_BYTE_INDEX_3, event[MIDI_MESSAGE_BYTE_INDEX_2]);
            trackNamesMap[eventTrackID] = trackName;
            break;
        }
            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 04 length text
            // Instrument name of length. This optional event is used to provide a textual clue regarding the intended instrumentation for a track (e.g. 'Piano' or 'Flute', etc). If used, it is reccommended to place this event near the start of a track)
        case 0x04: {
            std::string instrumentName = eventText(event, MIDI_MESSAGE_BYTE_INDEX_3, event[MIDI_MESSAGE_BYTE_INDEX_2]);
            // If prefix is set, appending name to track
            if (currentChannelPrefix != NO_CHANNEL_PREFIX) {
                auto track = getTrack(eventTrackID, currentChannelPrefix);
                track->instrumentName = instrumentName;
            }
            break;
        }
            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 20 01 сс
            // Midi channel prefix (cc is a byte specifying the MIDI channel (0-15). This optional event is used to associate any subsequent SysEx and Meta events with a particular MIDI channel, and will remain in effect until the next MIDI Channel Prefix Meta event or the next MIDI event)
        case 0x20: {
            currentChannelPrefix = event[MIDI_MESSAGE_BYTE_INDEX_3];
            break;
        }
            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 2F 00
            // End of track
        case 0x2F: {
            for (auto &track: tracksMap) {
                if(track.second->trackId == eventTrackID) {
                    track.second->closeAllNotes(eventTick);
                }
            }
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 51 03 tt tt tt
            // Tempo (tt tt tt is a 24-bit value specifying the tempo as the number of microseconds per quarter note)
        case 0x51: {
			int microsecondsPerQuarter = event[MIDI_MESSAGE_BYTE_INDEX_3] << 16 | 
									     event[MIDI_MESSAGE_BYTE_INDEX_4] << 8  | 
				                         event[MIDI_MESSAGE_BYTE_INDEX_5];
			// Assign beatsPerMinute only ones
            if (beatsPerMinute < 0) {
                beatsPerMinute = 1.0 * MICROSECONDS_IN_MINUTE / microsecondsPerQuarter;
                ticksPerBeat   =  ticksPerSecond * SECONDS_IN_MINUTE / beatsPerMinute;
                beatsPerTick =   1.0 / ticksPerBeat;
            }
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 20 01 сс
            // Key signature (sf is a byte specifying the number of flats (-ve) or sharps (+ve) that identifies the key signature (-7 = 7 flats, -1 = 1 flat, 0 = key of C, 1 = 1 sharp, etc). mi is a byte specifying a major (0) or minor (1) key)
        case 0x59: {
            break;
        }

        default: {
            break;
        }
        }
    } else {
        currentChannelPrefix = NO_CHANNEL_PREFIX; // If event is midi-event, channel prefix removes
        int command = event.getCommandNibble(); // P0 & 0x11110000
        auto currentTrack = getTrack(eventTrackID, eventChannelID);

        switch (command) {

        // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // 8n note velocity
        // Note OFF (Stop sounding the specified note, on MIDI channel n)
        case 0x80: {
            currentTrack->closeNote(event.getKeyNumber(), eventTick);
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // 9n note velocity
            // Note ON (Start sounding the specified note, on MIDI channel n)
        case 0x90: {
            //            debugEvent(event);
            int keyNumber = event.getKeyNumber();
            int velocity = event.getVelocity();
            if (velocity == 0) { // same as Note OFF
                currentTrack->closeNote(keyNumber, eventTick);
            } else {
                currentTrack->openNote(keyNumber, eventTick, velocity);
            }
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // An note pressure
            // Polyphonic Pressure (Apply aftertouch pressure to the specified note, on MIDI channel n)
        case 0xA0: {
            // TODO: Handle this shity staff
            //currentTrack->isPolyphonical = true;
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // Bn controller value
            // Controller (Set the specified controller, on MIDI channel n, to value)
        case 0xB0: {
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // Cn program
            // Program Change (Select the specified program (i.e. voice, or instrument), on MIDI channel n)
        case 0xC0: {
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // Dn pressure
            // Channel Pressure (Apply aftertouch pressure to all notes currently sounding on MIDI channel n)
        case 0xD0: {
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // En lsb msb
            // Pitch Bend (Apply pitch bend to all notes currently sounding on MIDI channel n)
        case 0xE0: {
            break;
        }
        default: {
            break;
        }
        }
    }
}

/*!
 * \brief MidiFileReader::getTrack
 *
 * Returns track with trackID and channelID, and if this track not exists, creates it
 * \param trackID
 * \param channelID
 * \return
 */
std::shared_ptr<MidiTrack> MidiFileReader::getTrack(const int trackID, const int channelID)
{
    int id = trackID * MAX_MIDI_CHANNELS + channelID;
    TrackMap::iterator it = tracksMap.find(id);
    if (it == tracksMap.end()) {
        std::shared_ptr<MidiTrack> track = std::make_shared<MidiTrack>();
        tracksMap[id] = track;
        track->trackId = trackID;
        track->channelId = channelID;
        return track;
    } else {
        return it->second;
    }
}

/*!
 * \brief MidiFileReader::eventText
 * Transformates n bytes from start to ASCII-std::string
 *
 * \param event
 * \param start
 * \param n
 * \return
 */
std::string MidiFileReader::eventText(const MidiEvent &event, const int start, const int n)
{
    size_t fin = start + n;
    if (fin > event.size()) {
        fin = event.size();
    }
    std::string s;
    for (size_t i = start; i < fin; i++) {
        s += (char)event[i];
    }
    return s;
}

/*!
 * \brief MidiFileReader::postProcess
 *
 * Counts necessary values for all stored MIDI tracks
 */
void MidiFileReader::postProcess()
{
    for (auto &trackMapItem : tracksMap) {
        std::shared_ptr<MidiTrack> track = trackMapItem.second;

        // Assigning track name
        TrackNamesMap::iterator it = trackNamesMap.find(track->trackId);
        if (it != trackNamesMap.end()) {
            track->trackName = it->second;
        }

        track->postProcess(ticksPerQuarter, durationInTicks);
        track->durationInTime = 1.0 * track->durationInTicks() / ticksPerSecond;
    }
}

/*!
 * \brief MidiFileReader::sortCompare
 *
 * Sort function to order tracks by the probabilities of being a vocal part
 * \param first
 * \param second
 * \return
 */
bool MidiFileReader::sortCompare(const std::shared_ptr<MidiTrack> &left, const std::shared_ptr<MidiTrack> &right)
{
    double lPoints = getSummaryWeight(left);
    double rPoints = getSummaryWeight(right);

    return lPoints > rPoints;
}

/*!
 * \brief MidiFileReader::getWeight
 *
 * Gets the weight of criteria value if it satisfies distribution with mean = mx and interval = sigma
 * \param value
 * \param baseWeight
 * \param mx
 * \param sigma
 * \return
 */
double MidiFileReader::getWeight(const double &value, const double &baseWeight, const double &mx, const double &sigma)
{
    bool isGood = satisfiesDistribution(value, mx, sigma);
    return isGood ? baseWeight : 0.0;
}

double MidiFileReader::getSummaryWeight(const std::shared_ptr<MidiTrack> &value) {
    double result = 0.0;

    if (containsTrackName(value->trackName + value->instrumentName)) {
        result += NAME_WEIGHT;
    }

    int noteCount = value->noteCount;
    if (noteCount >= MIN_NOTE_COUNT && noteCount <= MAX_NOTE_COUNT) {
        result += NOTE_COUNT_WEIGHT;
    }

    // Lowest note distribution
    result += getWeight(value->lowestNote,  LOWEST_NOTE_WEIGHT, LOWEST_NOTE_MX, LOWEST_NOTE_SIGMA);

    // Highest note distribution
    result += getWeight(value->highestNote,  HEIGHEST_NOTE_WEIGHT, HEIGHEST_NOTE_MX, HEIGHEST_NOTE_SIGMA);

    // Note range distribution
    result += getWeight(value->highestNote - value->lowestNote, RANGE_OF_NOTES_WEIGHT, RANGE_OF_NOTES_MX, RANGE_OF_NOTES_SIGMA);

    // Note values distribution
    result += getWeight(value->maxNoteValuePercent, MAX_NOTE_VALUE_WEIGHT, MAX_NOTE_VALUE_MX, MAX_NOTE_VALUE_SIGMA);

    // Notes per second distribution
    result += getWeight(noteCount / value->durationInTime, NOTES_PER_SECOND_WEIGHT, NOTES_PER_SECOND_MX, NOTES_PER_SECOND_SIGMA);

    return result;
}

/*!
 * \brief MidiFileReader::containsTrackName
 *
 * Returns if name contains some of widely-used vocal trackname substd::string
 * \param name
 * \return
 */
bool MidiFileReader::containsTrackName(const std::string &name)
{
    std::string s = name;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return     s.find(SUBSTRING_VOCAL)  != std::string::npos
            || s.find(SUBSTRING_LEAD)   != std::string::npos
            || s.find(SUBSTRING_SONG)   != std::string::npos
            || s.find(SUBSTRING_MELODY) != std::string::npos;
}

/*!
 * \brief MidiFileReader::satisfiesDistribution
 *
 * Returns true if value satisfies distribution with meaning value mx and sigma interval
 * \param value
 * \param mid
 * \param sko
 * \return
 */
bool MidiFileReader::satisfiesDistribution(const double &value, const double &mx, const double &sigma)
{
    return (value >= mx - sigma)
            && (value <= mx + sigma);
}

int MidiFileReader::tickToBeat(const int tick)
{
    return tick / ticksPerBeat;
}

double MidiFileReader::getBeatsPerMinute() const {
    return beatsPerMinute;
}

VocalPart MidiFileReader::tryGetVocalPartFromMidiTrackWithId(int midiTrackId) const {
    assert(beatsPerMinute >= 0 && "The midifile has not been parsed, call read before tryGetVocalPartFromMidiTrackWithId");

    auto iter = std::find_if(tracksMap.begin(), tracksMap.end(), [=] (const auto& pair) {
        return pair.second->trackId == midiTrackId;
    });

    if (iter == tracksMap.end()) {
        throw MidiFileReaderException(MidiFileReaderException::OUT_OF_RANGE);
    }

    std::shared_ptr<MidiTrack> track = iter->second;

    // Check if track satisfies base conditions
    if (track->channelId == DRUMS_CHANNEL_ID) {
        throw MidiFileReaderException(MidiFileReaderException::DRUM_TRACK);
    } else if(!Pitch::isMidiPitchSupported(track->highestNote)
              || !Pitch::isMidiPitchSupported(track->lowestNote)) {
        throw MidiFileReaderException(MidiFileReaderException::HAS_UNSUPPORTED_PITCHES);
    } else if(track->polyphonicTracksCount > MAX_SUPPORTED_POLYPHONIC_TRACKS_COUNT) {
        throw MidiFileReaderException(MidiFileReaderException::OUT_OF_MAX_SUPPORTED_POLYPHONIC_TRACKS_COUNT);
    }

    std::vector<NoteInterval> pitches;
    const auto &notes = track->getNotes();
    pitches.reserve(notes.size());
    for (const auto &note : notes) {
        NoteInterval pitch;
        pitch.pitch = Pitch::fromMidiIndex(note->keyNumber);
        pitch.startTickNumber = note->startTick;
        pitch.ticksCount = note->durationInTicks();
        pitches.push_back(pitch);
    }

    return VocalPart(std::move(pitches),
                     durationInTicks - track->finalTick, //endSilenceLengthInTicks
                     ticksPerSecond);
}
