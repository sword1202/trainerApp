#ifndef VXFILE_LYRICS_INTERVAL_H
#define VXFILE_LYRICS_INTERVAL_H

struct VxLyricsInterval {
    int startTimestampInMilliseconds;
    int endTimestampInMilliseconds;
    std::string lyrics;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & lyrics;
        ar & startTimestampInMilliseconds;
        ar & endTimestampInMilliseconds;
    }
};

#endif //VXFILE_LYRICS_INTERVAL_H
