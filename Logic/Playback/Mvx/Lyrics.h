//
// Created by Semyon Tikhonenko on 2019-02-11.
//

#ifndef TEXTIMAGESGENERATOR_LYRICS_H
#define TEXTIMAGESGENERATOR_LYRICS_H

#include "boostext/serialization/static_vector.h"
#include "LyricsLine.h"

class Lyrics {
    boost::container::static_vector<LyricsLine, 2> lyrics;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & lyrics;
    }
public:
    Lyrics() = default;
    explicit Lyrics(const LyricsLine& lyricsLine);
    Lyrics(const LyricsLine& lyricsLine1, const LyricsLine& lyricsLine2);

    int getLinesCount() const;
    const std::string& getLineName(int index) const;
    const std::string &getCurrentLyricsTextAtLine(int lineIndex, double time) const;
};


#endif //TEXTIMAGESGENERATOR_LYRICS_H
