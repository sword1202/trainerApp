/*
 * libaudiodecoder - Native Portable Audio Decoder Library
 * libaudiodecoder API Header File
 * Latest version available at: http://www.oscillicious.com/libaudiodecoder
 *
 * Copyright (c) 2010-2012 Albert Santoni, Bill Good, RJ Ryan  
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire libaudiodecoder license; however, 
 * the Oscillicious community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also 
 * requested that these non-binding requests be included along with the 
 * license above.
 */

#include "audiodecoder.h"
#include "WAVFile.h"

#include <memory>

#if defined(__APPLE__)
#include "audiodecodercoreaudio_mac.h"
#include "DecodedTrack.h"

#elif defined(_WIN32)
#include "audiodecodermediafoundation_win.h"
#else
#include"audiodecoderffmpeg.h"
#endif

int    AudioDecoder::numSamples()        const { return m_iNumSamples; }

int    AudioDecoder::channels()          const { return m_iChannels; }

int    AudioDecoder::sampleRate()        const { return m_iSampleRate; }

float  AudioDecoder::duration()          const { return m_fDuration; }

int    AudioDecoder::positionInSamples() const { return m_iPositionInSamples; }

AudioDecoder *AudioDecoder::create() {
#if defined(__APPLE__)
    return new AudioDecoderCoreAudio();
#elif defined(_WIN32)
    return new AudioDecoderMediaFoundation();
#else
    return new AudioDecoderFFmpeg();
#endif
}

static const int MP3_FRAME_COMMON_SIZE = 1152;

DecodedTrack
AudioDecoder::decodeAllIntoRawPcm(const std::string &data, const std::function<void(float)> &progressListener,
                                  OperationCancelerPtr operationCanceller) {
    std::unique_ptr<AudioDecoder> decoder(AudioDecoder::create());
    decoder->open(&data);

    WavConfig wavConfig;
    wavConfig.bitsPerChannel = 16;
    wavConfig.numberOfChannels = static_cast<unsigned int>(decoder->channels());
    wavConfig.sampleRate = 44100;
    std::string result;
    size_t totalSize = decoder->numSamples() * sizeof(int16_t);
    // reserve a bit more
    result.reserve(totalSize + 1000);

    int16_t buffer[MP3_FRAME_COMMON_SIZE];
    int readSamplesCount = MP3_FRAME_COMMON_SIZE;
    while (readSamplesCount >= MP3_FRAME_COMMON_SIZE) {
        if (operationCanceller && operationCanceller->isCancelled()) {
            return {result, wavConfig};
        }

        readSamplesCount = decoder->read(MP3_FRAME_COMMON_SIZE, buffer);
        result.append(reinterpret_cast<char*>(buffer), readSamplesCount * sizeof(int16_t));
        if (progressListener) {
            float progress = std::min(float(result.size()) / totalSize, 1.0f);
            progressListener(progress);
        }
    }

    return {result, wavConfig};
};


