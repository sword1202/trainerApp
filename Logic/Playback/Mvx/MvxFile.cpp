//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxFile.h"
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "audiodecoder.h"
#include <VocalPartAudioDataGenerator.h>
#include "StringUtils.h"
#include "AudioUtils.h"

constexpr int MAX_SAMPLES_PREVIEW_COUNT = 5000;

using namespace CppUtils;

void MvxFile::writeToStream(std::ostream &os) const {
    boost::archive::text_oarchive ar(os);
    ar << *this;
}

void MvxFile::writeToFile(const char *outFilePath) const {
    std::fstream file = Streams::OpenFile(outFilePath, std::ios::binary | std::ios::out);
    writeToStream(file);
}

void MvxFile::readFromStream(std::istream &is, bool readOnlySignature) {
    this->readOnlySignature = readOnlySignature;
    boost::archive::text_iarchive ar(is);
    ar >> *this;
}

void MvxFile::readFromFile(const char *filePath, bool readOnlySignature) {
    std::fstream file = Streams::OpenFile(filePath, std::ios::binary | std::ios::in);
    readFromStream(file, readOnlySignature);
}

const VocalPart &MvxFile::getVocalPart() const {
    return vocalPart;
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

double MvxFile::getScore() const {
    return score;
}

void MvxFile::setScore(double score) {
    assert(score <= 100);
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

const std::string &MvxFile::getRecordingData() const {
    return recordingData;
}

void MvxFile::setInstrumental(const std::string &instrumental) {
    this->instrumental = instrumental;
}

bool MvxFile::isRecording() const {
    return recording;
}

void MvxFile::setRecordingData(const std::string &recordingData) {
    MvxFile::recordingData = recordingData;
    recording = !recordingData.empty();
}

void MvxFile::setVocalPart(const VocalPart &vocalPart) {
    this->vocalPart = vocalPart;
}

const std::vector<double> &MvxFile::getRecordedPitchesTimes() const {
    return recordedPitchesTimes;
}

void MvxFile::setRecordedPitchesTimes(const std::vector<double> &recordedPitchesTimes) {
    MvxFile::recordedPitchesTimes = recordedPitchesTimes;
}

const std::vector<float> &MvxFile::getRecordedPitchesFrequencies() const {
    return recordedPitchesFrequencies;
}

void MvxFile::setRecordedPitchesFrequencies(const std::vector<float > &recordedPitchesFrequencies) {
    MvxFile::recordedPitchesFrequencies = recordedPitchesFrequencies;
}

const std::vector<short> &MvxFile::getInstrumentalPreviewSamples() const {
    return instrumentalPreviewSamples;
}

void MvxFile::setInstrumentalPreviewSamples(const std::vector<short> &instrumentalPreviewSamples) {
    this->instrumentalPreviewSamples = instrumentalPreviewSamples;
}

void MvxFile::generateInstrumentalPreviewSamplesFromInstrumental() {
    DecodedTrack decoded = AudioDecoder::decodeAllIntoRawPcm(instrumental);
    int previewSamplesCount = std::min(MAX_SAMPLES_PREVIEW_COUNT, int(decoded.rawPcm.size() / sizeof(short)));
    std::vector<short> previewSamples = AudioUtils::ResizePreviewSamples(decoded.rawPcm, previewSamplesCount);
    setInstrumentalPreviewSamples(previewSamples);
}

const Lyrics &MvxFile::getLyrics() const {
    return lyrics;
}

void MvxFile::setLyrics(const Lyrics &lyrics) {
    this->lyrics = lyrics;
}

static std::vector<short> makeVocalMultiChannel(const std::vector<short>& vocal, int channelsCount) {
    assert(channelsCount >= 2);
    size_t initialSize = vocal.size();
    std::vector<short> result(initialSize * channelsCount);
    for (int i = 0; i < initialSize; ++i) {
        for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex) {
            result[i * channelsCount + channelIndex] = vocal[i];
        }
    }

    return result;
}

std::string MvxFile::convertInstrumentalAndVocalTrackToWav(float vocalVolume) const {
    DecodedTrack decoded = AudioDecoder::decodeAllIntoRawPcm(instrumental);
    if (decoded.wavConfig.bitsPerChannel != sizeof(short) * 8) {
        throw std::runtime_error("Unsupported instrumental bitsPerChannel");
    }

    VocalPartAudioDataGeneratorConfig config;
    config.sampleRate = decoded.wavConfig.sampleRate;
    VocalPartAudioDataGenerator generator(vocalPart, config);
    std::vector<short> vocal = generator.readAll();
    int numberOfChannels = decoded.wavConfig.numberOfChannels;
    if (numberOfChannels >= 2) {
        vocal = makeVocalMultiChannel(vocal, numberOfChannels);
    }
    
    std::string result = WAVFile::addWavHeaderToRawPcmData<std::string>("", 0, decoded.wavConfig);
    const short* instrumental = reinterpret_cast<const short *>(decoded.rawPcm.data());
    int instrumentalSize = int(decoded.rawPcm.size()) / sizeof(short);
    if (vocal.size() < instrumentalSize) {
        vocal.resize(size_t(instrumentalSize), 0);
    }
    result.resize(WAVFile::DATA_POSITION + instrumentalSize * sizeof(short));

    AudioUtils::Mix2Sounds(instrumental, vocal.data(), instrumentalSize,
            (short *) (result.data() + WAVFile::DATA_POSITION), 1.0f, vocalVolume);

    return result;
}

const std::map<double, int> &MvxFile::getRecordingTonalityChanges() const {
    return recordingTonalityChanges;
}

void MvxFile::setRecordingTonalityChanges(const std::map<double, int> &recordingTonalityChanges) {
    this->recordingTonalityChanges = recordingTonalityChanges;
}

Lyrics &MvxFile::getLyricsNonConst() {
    return lyrics;
}
