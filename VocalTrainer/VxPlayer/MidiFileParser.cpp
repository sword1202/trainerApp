//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "midifileparser.h"
#include "MidiFile.h"

#include <iostream>
#include <algorithm>

MidiFileReader::MidiFileReader()
{

}

MidiFileReader::~MidiFileReader()
{

}

/*!
 * \brief MidiFileReader::read
 *
 *
 * \param filename
 * \return
 */
std::vector<VxFile> MidiFileReader::read(const string &filename)
{
    MidiFile f;
    if (f.read(filename) != 0) {
        return processMidiFile(f);
    } else {
        return std::vector<VxFile>();
    }
}

std::vector<VxFile> MidiFileReader::read(istream &is)
{
    MidiFile f;
    if (f.read(is) != 0) {
        return processMidiFile(f);
    } else {
        return std::vector<VxFile>();
    }
}

void MidiFileReader::clear()
{
    m_tickCount = 0;
    m_durationInSeconds = 0.0;
    m_durationInTicks = 0;
    m_tpq = 0;
    m_tps = 0.0;
    m_currentChannelPrefix = -1;
    m_tracksMap.clear();
    m_trackNamesMap.clear();
}

/*!
 * \brief MidiFileReader::processMidiFile
 *
 * Processes MidiFile object
 * \param midi
 * \return
 */
std::vector<VxFile> MidiFileReader::processMidiFile(MidiFile &midi)
{
    clear();
    m_durationInSeconds = midi.getTotalTimeInSeconds();
    m_durationInTicks = midi.getTotalTimeInTicks();
    m_tpq = midi.getTPQ();
    m_tps = m_durationInTicks / m_durationInSeconds;

    int trackCount = midi.getTrackCount();
    for (int track = 0; track < trackCount; ++ track) {
        for (int event = 0; event < midi[track].size(); ++ event) {
            processEvent(midi[track][event]);
        }
    }
    postProcess();
    auto trackList = getAvilableTracks();
    std::vector<VxFile> result;
    for (auto track : trackList) {
        std::vector<VxPitch> pitches;
        for (auto note : track->getNotes()) {
            VxPitch pitch;
            pitch.pitch = Pitch::fromMidiIndex(note->keyNumber);
            pitch.startBitNumber = note->startTick;
            pitch.bitsCount = note->durationInTicks();
            pitches.push_back(pitch);
        }
        VxFile f(pitches, m_durationInTicks - track->finalTick, m_tps * 60.0);
        result.push_back(f);
    }
    return result;
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
            std::string trackName = eventText(event, 3, event[2]);
            m_trackNamesMap[eventTrackID] = trackName;
            break;
        }
            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 04 length text
            // Instrument name of length. This optional event is used to provide a textual clue regarding the intended instrumentation for a track (e.g. 'Piano' or 'Flute', etc). If used, it is reccommended to place this event near the start of a track)
        case 0x04: {
            std::string instrumentName = eventText(event, 3, event[2]);
            // If prefix is set, appending name to track
            if (m_currentChannelPrefix != -1) {
                auto track = getTrack(eventTrackID, m_currentChannelPrefix);
                track->instrumentName = instrumentName;
            }
            break;
        }
            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 20 01 сс
            // Midi channel prefix (cc is a byte specifying the MIDI channel (0-15). This optional event is used to associate any subsequent SysEx and Meta events with a particular MIDI channel, and will remain in effect until the next MIDI Channel Prefix Meta event or the next MIDI event)
        case 0x20: {
            m_currentChannelPrefix = event[3];
            break;
        }
            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 2F 00
            // End of track
        case 0x2F: {
            for (auto track: m_tracksMap) {
                if(track.second->trackId == eventTrackID) {
                    track.second->closeAllNotes(eventTick);
                    track.second->finalTick = eventTick;
                }
            }
            break;
        }

            // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // FF 51 03 tt tt tt
            // Tempo (tt tt tt is a 24-bit value specifying the tempo as the number of microseconds per quarter note)
        case 0x51: {
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
        m_currentChannelPrefix = -1; // If event is midi-event, channel prefix removes
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
            currentTrack->isPolyphonical = true;
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
std::shared_ptr<MidiTrack> MidiFileReader::getTrack(int trackID, int channelID)
{
    int id = trackID * 15 + channelID;
    TrackMap::iterator it = m_tracksMap.find(id);
    if (it == m_tracksMap.end()) {
        std::shared_ptr<MidiTrack> track = std::make_shared<MidiTrack>();
        m_tracksMap[id] = track;
        track->trackId = trackID;
        track->channelId = channelID;
        return track;
    } else {
        return it->second;
    }
}

/*!
 * \brief MidiFileReader::eventText
 * Transformates n bytes from start to ASCII-string
 *
 * \param event
 * \param start
 * \param n
 * \return
 */
string MidiFileReader::eventText(const MidiEvent &event, int start, int n)
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
    for (auto trackMapItem : m_tracksMap) {
        std::shared_ptr<MidiTrack> track = trackMapItem.second;

        // Assigning track name
        TrackNamesMap::iterator it = m_trackNamesMap.find(track->trackId);
        if (it != m_trackNamesMap.end()) {
            track->trackName = it->second;
        }

        track->postProcess(m_tpq, m_durationInTicks);
        track->durationInTime = 1.0 * track->durationInTicks() / m_tps;
    }
}

/*!
 * \brief MidiFileReader::getTracks
 *
 * Gets track list that satisfies conditions
 * \return
 */
std::vector<std::shared_ptr<MidiTrack> > MidiFileReader::getAvilableTracks()
{
    std::vector<std::shared_ptr<MidiTrack> > tracks;

    // If duration more than 1 hour, return nothing
    if (m_durationInSeconds > 3600.0)
        return tracks;
    for (auto trackMapItem : m_tracksMap) {
        // Throwing out drums, empty and polyphonical tracks;
        std::shared_ptr<MidiTrack> track = trackMapItem.second;
        if ((track->noteCount > 40)
                && (track->noteCount < 500)
                && (track->channelId != 9)
                && (!track->isPolyphonical)
                ) {
            tracks.push_back(track);
        }
    }
    std::sort(tracks.begin(), tracks.end(), sortAlghorythm);
    return tracks;
}

/*!
 * \brief MidiFileReader::sortAlghorythm
 *
 * Algorythm to order tracks by the possibilities of being vocal
 * \param first
 * \param second
 * \return
 */
bool MidiFileReader::sortAlghorythm(const std::shared_ptr<MidiTrack> &left, const std::shared_ptr<MidiTrack> &right)
{
    // Comparing names
    // Returns left > right, so best result must be in right to be true
    std::string lTrackName = left->trackName + left->instrumentName;
    std::string rTrackName = right->trackName + right->instrumentName;

    double lPoints = 0.0;
    double rPoints = 0.0;
    double nameWeight = 3.0;
    double lowestNoteWeight = 1.0;
    double heighestNoteWeight = 1.0;
    double diapasoneWeight = 1.0;
    double maxNoteLengthWeight = 1.0;
    double npsWeight = 1.0;

    bool lContainsTrackName = containsTrackName(lTrackName);
    bool rContainsTrackName = containsTrackName(rTrackName);

    lPoints += lContainsTrackName * nameWeight;
    rPoints += rContainsTrackName * nameWeight;

    // Lowest note distribution
    lPoints += getWeight(left->lowestNote, lowestNoteWeight, 58.5, 9.25);
    rPoints += getWeight(right->lowestNote, lowestNoteWeight, 58.5, 9.25);

    // Highest note distribution
    lPoints += getWeight(left->highestNote, heighestNoteWeight, 58.5, 9.25);
    rPoints += getWeight(right->highestNote, heighestNoteWeight, 58.5, 9.25);

    // Diapasone distribution
    lPoints += getWeight(left->highestNote - left->lowestNote, diapasoneWeight, 16, 7);
    rPoints += getWeight(right->highestNote - right->lowestNote, diapasoneWeight, 16, 7);

    // Note length distribution
    lPoints += getWeight(left->maxNoteLengthPercent, maxNoteLengthWeight, 63, 21);
    rPoints += getWeight(right->maxNoteLengthPercent, maxNoteLengthWeight, 63, 21);

    // Notes per second distribution
    lPoints += getWeight(left->noteCount / left->durationInTime, npsWeight, 2.43, 1.95);
    rPoints += getWeight(right->noteCount / right->durationInTime, npsWeight, 2.43, 1.95);

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
    return baseWeight * isGood;
}

/*!
 * \brief MidiFileReader::containsTrackName
 *
 * Returns if name contains some of widely-used vocal trackname substring
 * \param name
 * \return
 */
bool MidiFileReader::containsTrackName(const string &name)
{
    std::string s = name;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s.find("vocal") != std::string::npos
            || s.find("lead") != std::string::npos
            || s.find("song") != std::string::npos
            || s.find("melod") != std::string::npos
            ;
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
    return value >= mx-sigma && value <= mx+sigma;
}
