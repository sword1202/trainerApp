//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxFile.h"
#include <fstream>
#include "audiodecoder.h"
#include "StringUtils.h"
#include "AudioUtils.h"
#include "BinaryArchive.h"

constexpr int MAX_SAMPLES_PREVIEW_COUNT = 5000;

using namespace CppUtils;

void MvxFile::writeToStream(std::ostream &os) const {
    os.write(MVX_SIGNATURE, MVX_SIGNATURE_LENGTH);
    Serialization::WriteObjectToBinaryStream(*this, os);
}

void MvxFile::writeToFile(const char *outFilePath) const {
    std::fstream file = Streams::OpenFile(outFilePath, std::ios::binary | std::ios::out);
    writeToStream(file);
}

void MvxFile::readFromStream(std::istream &is) {
    char signature[MVX_SIGNATURE_LENGTH];
    is.read(signature, MVX_SIGNATURE_LENGTH);
    if (strncmp(signature, MVX_SIGNATURE, (size_t) MVX_SIGNATURE_LENGTH)) {
        throw std::runtime_error("Invalid mvx file signature");
    }

    Serialization::ReadObjectFromBinaryStream(*this, is);
}

void MvxFile::readFromFile(const char *filePath) {
    std::fstream file = Streams::OpenFile(filePath, std::ios::binary | std::ios::in);
    readFromStream(file);
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
    instrumental.reset(new StdStringAudioDataBuffer(std::move(Strings::StreamToString(is))));
}

void MvxFile::loadInstrumentalFromFile(const char *filePath) {
    std::fstream file = Streams::OpenFile(filePath, std::ios::binary | std::ios::in);
    loadInstrumentalFromStream(file);
}

AudioDataBufferConstPtr MvxFile::getInstrumental() const {
    return instrumental;
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

AudioDataBufferConstPtr MvxFile::getRecordingData() const {
    return recordingData;
}

void MvxFile::setInstrumental(const std::string &instrumental) {
    this->instrumental.reset(new StdStringAudioDataBuffer(instrumental));
}

bool MvxFile::isRecording() const {
    return recording;
}

void MvxFile::setRecordingData(const std::string &recordingData) {
    this->recordingData.reset(new StdStringAudioDataBuffer(recordingData));
    recording = !recordingData.empty();
}

void MvxFile::setRecordingData(AudioDataBufferConstPtr recordingData) {
    this->recordingData = recordingData;
    recording = recordingData != nullptr;
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

const std::map<double, int> &MvxFile::getRecordingTonalityChanges() const {
    return recordingTonalityChanges;
}

void MvxFile::setRecordingTonalityChanges(const std::map<double, int> &recordingTonalityChanges) {
    this->recordingTonalityChanges = recordingTonalityChanges;
}

void MvxFile::loadLyricsFromFile(const char *filePath) {
    lyrics = Lyrics(Strings::ReadFileIntoString(filePath), &vocalPart);
}

void MvxFile::loadLyricsFromStream(std::istream &is) {
    lyrics = Lyrics(Strings::StreamToString(is));
}

const Tonality &MvxFile::getOriginalTonality() const {
    return originalTonality;
}

void MvxFile::setOriginalTonality(const Tonality &originalTonality) {
    MvxFile::originalTonality = originalTonality;
}

double MvxFile::getRecordingTempoFactor() const {
    return recordingTempoFactor;
}

void MvxFile::setRecordingTempoFactor(double tempoFactor) {
    MvxFile::recordingTempoFactor = tempoFactor;
}

MvxFile::MvxFile(const VocalTrainerFile* file) {
    setVocalPart(file->getVocalPart());
    setOriginalTonality(file->getOriginalTonality());
    setLyrics(file->getLyrics());
    setArtistNameUtf8(file->getArtistNameUtf8());
    setSongTitleUtf8(file->getSongTitleUtf8());
    setBeatsPerMinute(file->getBeatsPerMinute());
    setInstrumental(file->getInstrumental());
    setInstrumentalPreviewSamples(file->getInstrumentalPreviewSamples());
    setRecordedPitchesFrequencies(file->getRecordedPitchesFrequencies());
    setRecordedPitchesTimes(file->getRecordedPitchesTimes());
    setRecordingData(file->getRecordingData());
    setRecordingTonalityChanges(file->getRecordingTonalityChanges());
    setRecordingTempoFactor(file->getRecordingTempoFactor());
}

void MvxFile::setInstrumental(AudioDataBufferConstPtr instrumental) {
    MvxFile::instrumental = instrumental;
}
