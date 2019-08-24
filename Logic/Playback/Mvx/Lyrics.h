//
// Created by Semyon Tikhonenko on 2019-02-11.
//

#ifndef TEXTIMAGESGENERATOR_LYRICS_H
#define TEXTIMAGESGENERATOR_LYRICS_H

#include "boostext/serialization/static_vector.h"
#include "LyricsPart.h"

class Lyrics {
public:
    static constexpr int MAX_PARTS_COUNT = 2;
    typedef boost::container::static_vector<std::string, MAX_PARTS_COUNT> Snapshot;
private:
    boost::container::static_vector<LyricsPart, MAX_PARTS_COUNT> lyrics;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & lyrics;
    }
public:

    Lyrics() = default;
    explicit Lyrics(const LyricsPart& lyricsLine);
    Lyrics(const LyricsPart& lyricsLine1, const LyricsPart& lyricsLine2);

    int getNumberOfParts() const;
    const LyricsPart& getLyricsPart(int index);
    const std::string& getPartName(int index) const;
    const std::string &getCurrentLyricsTextForPart(int partIndex, double time) const;

    Snapshot getCurrentSnapshot(double time) const;

    bool operator==(const Lyrics &rhs) const;
    bool operator!=(const Lyrics &rhs) const;

    void addLyricsInterval(int lineIndex, const LyricsInterval& interval);
};


#endif //TEXTIMAGESGENERATOR_LYRICS_H
