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
 * requested that these non-binding requests be included aint with the 
 * license above.
 */


#ifndef __AUDIODECODERBASE_H__
#define __AUDIODECODERBASE_H__

#include <string>
#include <vector>

//#ifdef _WIN32
//#define DllExport   __declspec( dllexport )
//#else
//#define DllExport
//#endif

//Types
typedef short SAMPLE;

//Error codes
#define AUDIODECODER_ERROR -1
#define AUDIODECODER_OK     0

/** 
A word on real-time safety: 
At present, all API calls are blocking and none are considered real-time safe. For best performance,
try to avoid calling read() or any other libaudiodecoder function from inside your audio callback.
*/

class AudioDecoder {
public:
    virtual ~AudioDecoder() = default;

    /** Opens the file for decoding */
    virtual void open(std::string &&data) = 0;

    /** Seek to a sample in the file */
    virtual int seek(int filepos) = 0;

    /** Read a maximum of 'size' samples of audio into buffer.
        Samples are always returned as 16-bit integers, with stereo interlacing.
        Returns the number of samples read. */
    virtual int read(int size, const SAMPLE *buffer) = 0;

    /** Get the number of audio samples in the file. This will be a good estimate of the
        number of samples you can get out of read(), though you should not rely on it
        being perfectly accurate always. (eg. it might be slightly inaccurate with VBR MP3s)*/
    int numSamples() const;

    /** Get the number of channels in the audio file */
    int channels() const;

    /** Get the sample rate of the audio file (samples per second) */
    int sampleRate() const;

    /** Get the duration of the audio file (seconds) */
    float duration() const;

    /** Get the current playback position in samples */
    int positionInSamples() const;

    /** Get a list of the filetypes supported by the decoder, by extension */
    virtual std::vector<std::string> supportedFileExtensions() = 0;

protected:
    std::string audioData;
    int m_iNumSamples = 0;
    int m_iChannels = 0;
    int m_iSampleRate = 0;
    float m_fDuration = 0; // in seconds
    int m_iPositionInSamples = 0; // in samples;

public:
    static AudioDecoder* create();
};

#endif //__AUDIODECODERBASE_H__
