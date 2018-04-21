#ifndef VXFILE_LYRICS_INTERVAL_H
#define VXFILE_LYRICS_INTERVAL_H

struct VxLyricsInterval {
    int startTimestampInMilliseconds;
    int endTimestampInMilliseconds;
    std::wstring lyrics;
};

#endif //VXFILE_LYRICS_INTERVAL_H
