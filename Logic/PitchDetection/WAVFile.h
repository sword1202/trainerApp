//
// Created by Semyon Tikhonenko on 8/19/16.
//

#ifndef PITCHDETECTION_WAVFILE_H
#define PITCHDETECTION_WAVFILE_H

#include <fstream>
#include <vector>

struct WavConfig {
    unsigned int sampleRate = 44100;
    unsigned int numberOfChannels = 1;
    unsigned int bitsPerChannel = 16;
};

class WAVFile {
    int sampleRate = 0;
    int numberOfChannels = 0;
    int bytesPerChannel = 0;
    int audioFormat = 0;
    std::istream* stream;
    bool deleteStreamOnDestructor = false;

    void init(std::istream *stream);
public:
    static const int DATA_POSITION = 44;
    static const int PCM_AUDIO_FORMAT = 1;

    WAVFile(const WAVFile&) = delete;
    WAVFile(std::istream* stream);
    WAVFile(const char* data, int size);
    int getSampleRate() const;
    int getNumberOfChannels() const;
    int getAudioFormat() const;

    bool isValid() const;
    int getBytesPerChannel() const;
    void moveToTheFirstDataByte();
    int readData(void *buf, int size);
    int readData(void *buf, int size, int seek);
    ~WAVFile();

    // tested only for 16 bits per channel.
    static std::vector<char> addWavHeaderToRawPcmData(const char* data, int size, const WavConfig& config);
    static std::vector<char> addWavHeaderToRawPcmData(const char* data, int size);

    static bool isWavFile(const char* data, int size);

    bool shouldDeleteStreamOnDestructor() const;
    void setDeleteStreamOnDestructor(bool deleteStreamOnDestructor);
};


#endif //PITCHDETECTION_WAVFILE_H
