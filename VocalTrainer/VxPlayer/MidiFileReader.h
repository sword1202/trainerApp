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

#include "VxFile.h"

class MidiEvent;
class MidiFile;

class MidiFileReader {

    int    tickCount;
    double durationInSeconds;
    int    durationInTicks;
    int    tpq;
    double tps;

    int    currentChannelPrefix;

    using TrackMap      = std::unordered_map<int, std::shared_ptr<MidiTrack > >;
    using TrackNamesMap = std::unordered_map<int, std::string>;
    TrackMap      tracksMap;     // key = 15 * trackID + channelID
    TrackNamesMap trackNamesMap; // key = trackID
public:
    explicit MidiFileReader();
    ~MidiFileReader();

    std::vector<VxFile> read(const std::string &filename);
    std::vector<VxFile> read(std::istream &is);

private:
    std::vector<VxFile> processMidiFile(MidiFile &midi);
    void processEvent(const MidiEvent &event);
    std::shared_ptr<MidiTrack> getTrack(const int trackID, const int channelID);
    std::string eventText(const MidiEvent &event, const int startByte, const int bytesAmount);
    void postProcess();
    std::vector<std::shared_ptr<MidiTrack> > getAvailableTracks();
    static bool sortCompare(const std::shared_ptr<MidiTrack> &first, const std::shared_ptr<MidiTrack> &second);
    static double getWeight(const double &value, const double &baseWeight, const double &mx, const double &sigma);

    static bool containsTrackName(const std::string &name);
    static bool satisfiesDistribution(const double &value, const double &mid, const double &sko);
};

#endif // MIDIFILEREADER_H
