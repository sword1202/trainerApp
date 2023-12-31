//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef MIDIFILEREADER_H
#define MIDIFILEREADER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <memory>
#include <iostream>

#include "MidiNote.h"
#include "MidiTrack.h"
#include "MidiFile.h"

#include "VocalPart.h"
#include "Tonality.h"
#include "TimeSignature.h"

class MidiFileReader {

    double durationInSeconds;
    int    durationInTicks;
    int    ticksPerQuarter;
    double ticksPerSecond;
    double beatsPerMinute;
    int    ticksPerBeat;
    double beatsPerTick;
    Tonality tonality;
    TimeSignature timeSignature;

    int    currentChannelPrefix;

    using TrackMap      = std::unordered_map<int, std::shared_ptr<MidiTrack > >;
    using TrackNamesMap = std::unordered_map<int, std::string>;
    TrackMap      tracksMap;     // key = 15 * trackID + channelID
    TrackNamesMap trackNamesMap; // key = trackID
public:
    explicit MidiFileReader();
    ~MidiFileReader();

    bool read(const std::string &filename);
	bool read(std::istream &is);

	double getBeatsPerMinute() const;

	const Tonality &getTonality() const;

	const TimeSignature &getTimeSignature() const;

	VocalPart tryGetVocalPartFromMidiTrackWithId(int midiTrackId) const;

private:
	void reset();
    void processMidiFile(MidiFile &midi);
    void processEvent(const MidiEvent &event);
    std::shared_ptr<MidiTrack> getTrack(const int trackID, const int channelID);
    std::string eventText(const MidiEvent &event, const int startByte, const int bytesAmount);
    void postProcess();
    static bool sortCompare(const std::shared_ptr<MidiTrack> &first, const std::shared_ptr<MidiTrack> &second);
    static double getWeight(const double &value, const double &baseWeight, const double &mx, const double &sigma);
    static double getSummaryWeight(const std::shared_ptr<MidiTrack> &value);

    static bool containsTrackName(const std::string &name);
    static bool satisfiesDistribution(const double &value, const double &mid, const double &sko);

    int tickToBeat(const int tick);

};

#endif // MIDIFILEREADER_H
