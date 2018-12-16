//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxFile.h"
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "StringUtils.h"

using namespace CppUtils;

MvxFile::MvxFile() {

}

MvxFile::MvxFile(VxFile &&vxFile, std::string &&instrumental, double beatsPerMinute) : vxFile(std::move(vxFile)),
                                                                                       instrumental(std::move(instrumental)),
                                                                                       beatsPerMinute(beatsPerMinute) {
}

void MvxFile::writeToStream(std::ostream &os) {
    boost::archive::text_oarchive ar(os);
    ar << *this;
}

void MvxFile::writeToFile(const char *outFilePath) {
    std::fstream file = Streams::OpenFile(outFilePath, std::ios::binary | std::ios::out);
    writeToStream(file);
}

MvxFile MvxFile::readFromStream(std::istream &is) {
    MvxFile file;
    boost::archive::text_iarchive ar(is);
    ar >> file;
    return file;
}

MvxFile MvxFile::readFromFile(const char *outFilePath) {
    std::fstream file = Streams::OpenFile(outFilePath, std::ios::binary | std::ios::in);
    return readFromStream(file);
}

const VxFile &MvxFile::getVxFile() const {
    return vxFile;
}

VxFile &MvxFile::getVxFile() {
    return vxFile;
}

double MvxFile::getBeatsPerMinute() const {
    return beatsPerMinute;
}

void MvxFile::setBeatsPerMinute(double beatsPerMinute) {
    this->beatsPerMinute = beatsPerMinute;
}

void MvxFile::loadInstrumentalFromStream(std::istream &is) {
    instrumental = Strings::StreamToString(is);
}

void MvxFile::loadInstrumentalFromFile(const char *filePath) {
    std::fstream file = Streams::OpenFile(filePath, std::ios::binary | std::ios::in);
    loadInstrumentalFromStream(file);
}

const std::string &MvxFile::getInstrumental() const {
    return instrumental;
}

std::string &MvxFile::getInstrumental() {
    return instrumental;
}

MvxFile::MvxFile(const VxFile &vxFile, std::istream &instrumentalStream, double beatsPerMinute) :
        beatsPerMinute(beatsPerMinute), vxFile(vxFile) {
    instrumental = Strings::StreamToString(instrumentalStream);
}

MvxFile::MvxFile(const VxFile &vxFile, const char *instrumentalFile, double beatsPerMinute) :
        beatsPerMinute(beatsPerMinute), vxFile(vxFile) {
    std::fstream file = Streams::OpenFile(instrumentalFile, std::ios::in | std::ios::binary);
    instrumental = Strings::StreamToString(file);
}

double MvxFile::getScore() const {
    return score;
}

void MvxFile::setScore(double score) {
    MvxFile::score = score;
}

const std::string &MvxFile::getSongTitleUtf8() const {
    return songTitleUtf8;
}

void MvxFile::setSongTitleUtf8(const std::string &songTitleUtf8) {
    MvxFile::songTitleUtf8 = songTitleUtf8;
}

const std::string &MvxFile::getArtistNameUtf8() const {
    return artistNameUtf8;
}

void MvxFile::setArtistNameUtf8(const std::string &artistNameUtf8) {
    MvxFile::artistNameUtf8 = artistNameUtf8;
}
