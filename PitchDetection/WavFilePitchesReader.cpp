//
// Created by Semyon Tikhonenko on 9/17/16.
//

#include "WavFilePitchesReader.h"
#include "AudioUtils.h"
#include "WavBufferReader.h"

static const int SAMPLE_SIZE = 1200;

WavFilePitchesReader::WavFilePitchesReader() {
    pitchDetector.init(SAMPLE_SIZE, 0);
}

std::vector<PitchDetection> WavFilePitchesReader::readPitchesFromWavFile(std::istream* stream, float threshold) {
    std::vector<PitchDetection> result;
    WavBufferReader bufferReader(stream, SAMPLE_SIZE);
    int summaryCount = 0;
    while (true) {
        int count = bufferReader.readDataIntoResultBuffer();
        if (count <= 0) {
            return result;
        }

        summaryCount += count;
        const WAVFile& wavFile = bufferReader.getWavFile();
        pitchDetector.setThreshold(threshold);
        assert(count <= bufferReader.getSampleSize());
        float frequency = pitchDetector.getFrequencyFromBuffer(bufferReader.getResultBuffer());

        PitchDetection pitchDetection;
        pitchDetection.pitch = Pitch(frequency);
        pitchDetection.durationInMicroSeconds = AudioUtils::GetSampleTimeInMicroseconds(SAMPLE_SIZE, wavFile.getSampleRate());
        result.push_back(pitchDetection);
    }
}
