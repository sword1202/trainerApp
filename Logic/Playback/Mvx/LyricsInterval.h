#ifndef VXFILE_LYRICS_INTERVAL_H
#define VXFILE_LYRICS_INTERVAL_H

#include <assert.h>

struct LyricsInterval {
    double startTime;
    double endTime;
    // utf-8
    std::string text;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & text;
        ar & startTime;
        ar & endTime;
        assert(startTime >= 0);
        assert(endTime >= 0);
    }

    bool containsTime(double time) const;
};

#endif //VXFILE_LYRICS_INTERVAL_H
