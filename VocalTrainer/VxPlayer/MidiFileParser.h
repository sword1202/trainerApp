//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef MIDIFILEPARSER_H
#define MIDIFILEPARSER_H

#include <string>
#include <map>
#include <vector>
#include <list>
#include <memory>
#include <iostream>

#include "midinote.h"
#include "miditrack.h"

#include "../VxFile.h"

class MidiEvent;
class MidiFile;
class MidiFileParser;
class VxFile;

class MidiFileReader {

    int    m_tickCount;
    double m_durationInSeconds;
    int    m_durationInTicks;
    int    m_tpq;
    double m_tps;

    int    m_currentChannelPrefix;

    using TrackMap      = std::map<int, std::shared_ptr<MidiTrack > >;
    using TrackNamesMap = std::map<int, std::string>;
    TrackMap      m_tracksMap;     // key = 15 * trackID + channelID
    TrackNamesMap m_trackNamesMap; // key = trackID
public:
    explicit MidiFileReader();
    virtual ~MidiFileReader();

    std::vector<VxFile> read(const std::string &filename);
    std::vector<VxFile> read(std::istream &is);

private:
    void clear();
    std::vector<VxFile> processMidiFile(MidiFile &midi);
    void processEvent(const MidiEvent &event);
    std::shared_ptr<MidiTrack> getTrack(int trackID, int channelID);
    std::string eventText(const MidiEvent &event, int start, int n);
    void postProcess();
    std::vector<std::shared_ptr<MidiTrack> > getAvilableTracks();
    static bool sortAlghorythm(const std::shared_ptr<MidiTrack> &first, const std::shared_ptr<MidiTrack> &second);
    static double getWeight(const double &value, const double &baseWeight, const double &mx, const double &sigma);

    static bool containsTrackName(const std::string &name);
    static bool satisfiesDistribution(const double &value, const double &mid, const double &sko);
};

#endif // MIDIFILEPARSER_H
